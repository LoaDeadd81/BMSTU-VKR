#include "PetriNetBuilder.h"

shared_ptr<PetriNet> PetriNetBuilder::build() {
    add_types(type_info);
    add_gens(type_info);
    add_reception(rec_info);
    add_groups(win_info);

    return make_shared<PetriNet>(p_to_t_arc, t_to_p_arc, p_num, timing, gen_type, q_pos, selector_t, win_poc,
                                 type_proc_distro);
}

StatSource PetriNetBuilder::get_stat_src() {
    return src;
}

void PetriNetBuilder::add_types(const vector<TypeInfo> &info) {
    for (auto &it: info) {
        type_proc_distro[it.type] = it.proc_distro;
    }
}

void PetriNetBuilder::add_gens(const vector<TypeInfo> &info) {
    int gen_num = type_info.size();

    for (int i = 0; i < gen_num; ++i) {
        t_to_p_arc.emplace_back(i, p_num);
        timing.push_back(info[i].gen_distro);
        gen_type.push_back(info[i].type);
    }

    p_num += 1;

    src.gen_num = gen_num;
}

void PetriNetBuilder::add_reception(const ReceptionInfo &info) {
    int t_num = timing.size();
    int cur_p_num = p_num - 1;

    p_to_t_arc.push_back({{cur_p_num, t_num}, false});
    p_to_t_arc.push_back({{cur_p_num, t_num + 2}, false});
    p_to_t_arc.push_back({{cur_p_num + 1, t_num + 1}, false});
    p_to_t_arc.push_back({{cur_p_num + 2, t_num}, false});
    p_to_t_arc.push_back({{cur_p_num + 2, t_num + 2}, true});

    t_to_p_arc.emplace_back(t_num, cur_p_num + 1);
    t_to_p_arc.emplace_back(t_num + 1, cur_p_num + 3);
    t_to_p_arc.emplace_back(t_num + 1, cur_p_num + 2);

    timing.push_back(make_shared<ImmediateDistribution>());
    timing.push_back(info.distro);
    timing.push_back(make_shared<ImmediateDistribution>());

    q_pos.push_back({cur_p_num + 2, info.max_q});

    p_num += 3;

    src.reception_q_src.in_t = t_num;
    src.reception_q_src.queue_p = cur_p_num + 1;
    src.reception_q_src.leave_t = t_num + 2;
    src.reception_src = t_num + 1;
}

void PetriNetBuilder::add_groups(const vector<WinGroupInfo> &info) {
    int select_from = p_num - 1;

    for (auto &it: info) {
        add_win_groups(it, select_from);
    }

    for (auto &it: win_poc) {
        t_to_p_arc.emplace_back(it, p_num);
    }

    p_to_t_arc.push_back({{p_num, timing.size()}, false});
    src.out_t = timing.size();

    p_num += 1;

    timing.push_back(make_shared<ImmediateDistribution>());
}

void PetriNetBuilder::add_win_groups(const WinGroupInfo &info, int select_from) {
    int t_num = timing.size();
    int cur_p_num = p_num - 1;

    p_to_t_arc.push_back({{select_from, t_num}, false});
    p_to_t_arc.push_back({{cur_p_num + 1, t_num + 1}, false});
    p_to_t_arc.push_back({{cur_p_num + 1, t_num + 2}, false});
    p_to_t_arc.push_back({{cur_p_num + 3, t_num + 1}, false});
    p_to_t_arc.push_back({{cur_p_num + 3, t_num + 2}, true});

    t_to_p_arc.emplace_back(t_num, cur_p_num + 1);
    t_to_p_arc.emplace_back(t_num + 1, cur_p_num + 2);

    timing.push_back(make_shared<ImmediateDistribution>());
    timing.push_back(make_shared<ImmediateDistribution>());
    timing.push_back(make_shared<ImmediateDistribution>());

    p_num += 3;

    q_pos.push_back({cur_p_num + 3, info.max_q});

    QueueStatSource q_src{};
    q_src.in_t = t_num;
    q_src.queue_p = cur_p_num + 2;
    q_src.leave_t = t_num + 2;
    vector<int> w_out;

    for (auto &it: info.types) {
        selector_t[t_num].insert(it);
    }

    for (int i = 0; i < info.win_num; ++i) {
        auto t_out = add_win(info, cur_p_num + 2, cur_p_num + 3);
        w_out.push_back(t_out);
    }

    src.window_q_src.push_back(q_src);

    WindowGroupStatSource w_src{w_out};
    src.window_src.push_back(w_src);
}

int PetriNetBuilder::add_win(const WinGroupInfo &info, int select_from, int q_p) {
    int t_num = timing.size();
    int cur_p_num = p_num - 1;

    p_to_t_arc.push_back({{select_from, t_num}, false});
    p_to_t_arc.push_back({{cur_p_num + 1, t_num + 1}, false});
    p_to_t_arc.push_back({{cur_p_num + 1, t_num}, true});

    t_to_p_arc.emplace_back(t_num, cur_p_num + 1);
    t_to_p_arc.emplace_back(t_num, q_p);

    timing.push_back(make_shared<ImmediateDistribution>());
    timing.push_back(make_shared<ImmediateDistribution>());

    p_num += 1;

    win_poc.insert(t_num + 1);

    return t_num + 1;
}
