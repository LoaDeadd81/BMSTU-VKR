#ifndef SRC_STATS_H
#define SRC_STATS_H

#include <utility>

#include "PetriNetBuilder.h"

struct SystemStats {
    int total_req;
    double complete_perc;
    double leave_perc;
};

struct QueueStats {
    int max{};
    int entries{};
    int empty_entries{};
    double avg_size{};
    double avg_time{};
    int leave_num{};

};

struct WorkerStats {
    int complete{};
    double util{};
    double avg_work_time{};
};

struct SMOStats {
    SystemStats system_stats;

    QueueStats reception_queue_stats;
    WorkerStats reception_stats;

    vector<QueueStats> window_groups_queue_stats;
    vector<WorkerStats> window_groups_stats;
};

class StatsAggregator {
private:
    int mtime;
    StatSource src;
    vector<T_Stats> t_stats;
    vector<P_Stats> p_stats;
public:
    StatsAggregator(int mtime, StatSource src, vector<T_Stats> t_stats, vector<P_Stats> p_stats)
            : mtime(mtime), src(std::move(src)), t_stats(std::move(t_stats)), p_stats(std::move(p_stats)) {};

    SMOStats get_stats();

private:
    SystemStats get_sys_stats();

    WorkerStats get_w_group_stats(const WindowGroupStatSource &g_src);

    QueueStats get_q_stats(QueueStatSource q_src);

    WorkerStats get_w_stats(int t_i);
};

#endif
