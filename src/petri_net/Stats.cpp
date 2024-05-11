#include "Stats.h"

SMOStats StatsAggregator::get_stats() {
    SMOStats stats;

    stats.system_stats = get_sys_stats();

    stats.reception_queue_stats = get_q_stats(src.reception_q_src);
    stats.reception_stats = get_w_stats(src.reception_src);

    stats.window_groups_queue_stats = vector<QueueStats>(src.window_q_src.size());
    stats.window_groups_stats = vector<WorkerStats>(src.window_q_src.size());
    for (int i = 0; i < src.window_q_src.size(); ++i) {
        stats.window_groups_queue_stats[i] = get_q_stats(src.window_q_src[i]);
        stats.window_groups_stats[i] = get_w_group_stats(src.window_src[i]);
    }

    return stats;
}

SystemStats StatsAggregator::get_sys_stats() {
    int total_req = 0;
    for (int i = 0; i < src.gen_num; ++i) total_req += t_stats[i].fire_num;

    int complete_num = t_stats[src.out_t].fire_num;
    double complete_perc = complete_num / double(total_req);

    int leave_num = t_stats[src.reception_q_src.leave_t].fire_num;
    for (auto &it: src.window_q_src) leave_num += t_stats[it.leave_t].fire_num;
    double leave_perc = leave_num / double(total_req);

    return SystemStats{total_req, complete_perc, leave_perc};
}

WorkerStats StatsAggregator::get_w_group_stats(const WindowGroupStatSource &g_src) {
    WorkerStats w_stats;

    int complete = 0;
    double util = 0;
    double avg_work_time = 0;
    for (auto it: g_src.out_t) {
        auto tmp = get_w_stats(it);
        complete += tmp.complete;
        util += tmp.util;
        avg_work_time += tmp.avg_work_time;
    }
    util /= double(g_src.out_t.size());
    avg_work_time /= double(g_src.out_t.size());

    return {complete, util, avg_work_time};
}

QueueStats StatsAggregator::get_q_stats(QueueStatSource q_src) {
    int p_i = q_src.queue_p;
    auto stats = p_stats[p_i];
    QueueStats q_stats = {};

    q_stats.max = stats.max_chip;
    q_stats.entries = stats.entries + t_stats[q_src.leave_t].fire_num;
    q_stats.empty_entries = stats.empty_entries;
    q_stats.avg_size = stats.avg;
    q_stats.leave_num = t_stats[q_src.leave_t].fire_num;

    double time = 0;
    for (int i = 0; i < stats.out_time.size(); ++i) time += stats.out_time[i] - stats.in_time[i];
    q_stats.avg_time = time / double(stats.out_time.size());

    return q_stats;
}

WorkerStats StatsAggregator::get_w_stats(int t_i) {
    auto stats = t_stats[t_i];
    WorkerStats w_stats;

    w_stats.complete = stats.fire_num;

    double work_time = 0;
    for (int i = 0; i < stats.fire_num; ++i) work_time += stats.gen_times[i];
    w_stats.avg_work_time = stats.fire_num > 0 ? work_time / double(stats.fire_num) : 0;
    w_stats.util = work_time / double(mtime);

    return w_stats;
}
