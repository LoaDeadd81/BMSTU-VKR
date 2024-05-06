#include "PetriNet.h"

#include <utility>
#include <iostream>

PetriNet::PetriNet(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_to_p_arc, int p_num, dist_vector timing,
                   vector<int> gen_type, const vector<Q_pos> &q_pos, unordered_map<int, unordered_set<int>> selector_t,
                   unordered_set<int> win_poc, unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro) {
    this->p_num = p_num;
    t_num = timing.size();

    this->p_to_t_arc = p_to_t_arc;
    this->t_to_p_arc = t_to_p_arc;

    this->win_poc = std::move(win_poc);
    this->type_proc_distro = std::move(type_proc_distro);

    for (int i = 0; i < gen_type.size(); ++i) gen_t[i] = gen_type[i];

    m = vector<deque<int>>(p_num);
    for (auto pos: q_pos) {
        q_p[pos.p_i] = true;
        for (int i = 0; i < pos.val; ++i)
            m[pos.p_i].push_back(-1);
    }
    this->selector_t = std::move(selector_t);

    t_consequences = vector<set<int>>(t_num, set<int>());
    t_check = vector<vector<ArcCheck>>(t_num, vector<ArcCheck>());
    t_effect = vector<vector<ArcEffect>>(t_num, vector<ArcEffect>());
    count(p_to_t_arc, t_to_p_arc);

    this->timing = std::move(timing);

    for (int i = 0; i < t_num; ++i) {
        is_wait[i] = false;
    }

    int type_num = gen_type.size();
    p_stats = vector<P_Stats>(p_num);
    for (int i = 0; i < p_num; ++i) {
        p_stats[i] = P_Stats{
                .max_chip = 0,
                .entries = 0,
                .empty_entries = 0,
                .sum = 0,
                .in_time = vector<double>(),
                .out_time = vector<double>(),
                .type_in_time = vector<vector<double>>(type_num),
                .type_out_time = vector<vector<double>>(type_num)
        };
    }

    t_stats = vector<T_Stats>(t_num);
    for (int i = 0; i < t_num; ++i) {
        t_stats[i] = T_Stats{
                .fire_num = 0,
                .fire_times = vector<double>(),
                .gen_times = vector<double>(),
                .type_fire_num = vector<int>(type_num),
                .type_fire_times = vector<vector<double>>(type_num),
                .type_gen_times = vector<vector<double>>(type_num)
        };
    }
}

void PetriNet::run(int limit) {
    int count = 0;
    queue<PetriEvent> q;
    auto to_fire = find_fired_t_init();
    for (auto it: to_fire) q.push(it);

    while (!q.empty() && count < limit) {
        auto event = q.back();
        q.pop();

        auto check_res = is_t_fire(event.t_i);
        if (!check_res.first) continue;

        fire_t(event);
        to_fire = find_fired_t(event.t_i);
        for (auto it: to_fire) q.push(it);

        count++;
    }
}

vector<PetriEvent> PetriNet::find_fired_t_init() {
    vector<PetriEvent> res;

    for (int i = 0; i < t_num; ++i) {
        if (is_wait[i]) continue;

        auto fire_res = is_t_fire(i);
        if (fire_res.first) {
            is_wait[i] = true;
            res.push_back({i, fire_res.second});
            cout << "to fire: " << i << " int: " << fire_res.second << endl;
        }
    }

    return res;
}

vector<PetriEvent> PetriNet::find_fired_t(int t_i) {
    vector<PetriEvent> res;

    for (auto const &i: t_consequences[t_i]) {
        if (is_wait[i]) continue;

        auto fire_res = is_t_fire(i);
        if (fire_res.first) {
            is_wait[i] = true;
            res.push_back({i, fire_res.second});
            cout << "to fire: " << i << " int: " << fire_res.second << endl;
        }
    }

    return res;
}

pair<bool, double> PetriNet::is_t_fire(int t_i) {
    if (selector_t.contains(t_i)) return check_selector_t(t_i);
    else if (win_poc.contains(t_i)) return check_win_proc_t(t_i);
    return check_usual_t(t_i);
}

void PetriNet::fire_t(PetriEvent event) {
    t_stats[event.t_i].fire_num++;

    int t_i = event.t_i;
    is_wait[t_i] = false;
    if (gen_t.contains(t_i)) {
        process_gen_t(event);
    } else if (selector_t.contains(t_i)) {
        process_selector_t(event);
    } else {
        process_usual_t(event);
    }

    cout << endl << endl;
    cout << "fired: " << t_i << endl;
    cout << "M: ";
    for (const auto &it: m) cout << it.size() << " ";
    cout << endl;
}

PetriNetImportData PetriNet::get_import_data() {
    unordered_map<int, int> q_info;
    for (auto &it: q_p) {
        if (it.second) q_info[it.first] = m[it.first].size();
    }

    return {p_num, t_num, p_to_t_arc, t_to_p_arc, gen_t, q_info, selector_t, win_poc};
}

pair<T_Stats, P_Stats> PetriNet::get_stats() {
    for (auto &it: logs) {
        auto &t_stat = t_stats[it.t];
        t_stat.fire_num++;
        t_stat.fire_times.push_back(it.sys_time);
        t_stat.gen_times.push_back(it.gen_time);
    }
}

pair<bool, double> PetriNet::check_selector_t(int t_i) {
    auto eff = cpn_t_effect[t_i];
    auto tokens = m[eff.pop_p];
    auto need_tokens = selector_t[t_i];

    for (auto &it: tokens) {
        if (need_tokens.contains(it)) return {true, timing[t_i]->gen()};
    }

    return {false, 0};
}

pair<bool, double> PetriNet::check_win_proc_t(int t_i) {
    auto eff = cpn_t_effect[t_i];
    auto tokens = m[eff.pop_p];

    if (tokens.empty()) return {false, 0};

    auto type = tokens.front();
    return {true, type_proc_distro[type]->gen()};
}

pair<bool, double> PetriNet::check_usual_t(int t_i) {
    for (auto const &it: t_check[t_i])
        if (m[it.p_index].size() < it.min_num || !m[it.p_index].empty() && it.is_ing)
            return {false, 0};

    return {true, timing[t_i]->gen()};
}

void PetriNet::process_gen_t(PetriEvent event) {
    int t_i = event.t_i;
    auto eff = cpn_t_effect[t_i];

    push_p_stats(eff.push_p);
    m[eff.push_p].push_back(gen_t[t_i]);
    logs.push_back({t_i, eff.pop_p, eff.push_p, gen_t[t_i], event.gen_time, event.sys_time});
}

void PetriNet::process_selector_t(PetriEvent event) {
    int t_i = event.t_i;
    auto eff = cpn_t_effect[t_i];
    auto &tokens = m[eff.pop_p];
    auto need_tokens = selector_t[t_i];

    int val = -1;
    for (auto it = begin(tokens); it != end(tokens); ++it) {
        if (need_tokens.contains(*it)) {
            val = *it;
            tokens.erase(it);
            break;
        }
    }

    if (eff.add_p >= 0) {
        if (eff.add_val > 0) {
            push_p_stats(eff.add_p);
            m[eff.add_p].push_back(-1);
            logs.push_back({t_i, -1, eff.add_p, -1, event.gen_time, event.sys_time});
        } else {
            m[eff.add_p].pop_front();
            logs.push_back({t_i, eff.add_p, -1, -1, event.gen_time, event.sys_time});
        }
    }

    int to = -1;
    if (eff.push_p >= 0) {
        push_p_stats(eff.push_p);
        m[eff.push_p].push_back(val);
        to = eff.push_p;
    }
    logs.push_back({t_i, eff.pop_p, to, val, event.gen_time, event.sys_time});
}

void PetriNet::process_usual_t(PetriEvent event) {
    int t_i = event.t_i;
    auto eff = cpn_t_effect[t_i];

    if (eff.add_p >= 0) {
        if (eff.add_val > 0) {
            push_p_stats(eff.add_p);
            m[eff.add_p].push_back(-1);
            logs.push_back({t_i, -1, eff.add_p, -1, event.gen_time, event.sys_time});
        } else {
            m[eff.add_p].pop_front();
            logs.push_back({t_i, eff.add_p, -1, -1, event.gen_time, event.sys_time});
        }
    }

    int val = m[eff.pop_p].front();
    m[eff.pop_p].pop_front();
    int to = -1;
    if (eff.push_p >= 0) {
        push_p_stats(eff.push_p);
        m[eff.push_p].push_back(val);
        to = eff.push_p;
    }
    logs.push_back({t_i, eff.pop_p, to, val, event.gen_time, event.sys_time});
}

void PetriNet::push_p_stats(int p_i) {
    int cur_chip = m[p_i].size();
    auto &stats = p_stats[p_i];

    stats.entries++;
    if (stats.max_chip < cur_chip + 1) stats.max_chip = cur_chip + 1;
    if (cur_chip == 0) stats.empty_entries++;
    stats.sum += cur_chip;
}

void PetriNet::count(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc) {
    auto r_minus = vector<vector<pair<int, bool>>>(p_num, vector<pair<int, bool>>(t_num, {0, false}));
    auto r_plus = vector<vector<int>>(p_num, vector<int>(t_num, 0));
    auto r_mtr = vector<vector<int>>(p_num, vector<int>(t_num, 0));

    for (const auto &it: p_to_t_arc)
        r_minus[it.arc.first][it.arc.second] = {1, it.is_ing};

    for (const auto &it: t_po_p_arc)
        r_plus[it.second][it.first] = 1;

    for (int i = 0; i < p_num; ++i)
        for (int j = 0; j < t_num; ++j)
            r_mtr[i][j] = r_plus[i][j] - (r_minus[i][j].second ? 0 : r_minus[i][j].first);


    for (int j = 0; j < t_num; ++j)
        for (int i = 0; i < p_num; ++i)
            if (r_minus[i][j].first != 0)
                t_check[j].push_back({
                                             i,
                                             r_minus[i][j].second ? 0 : r_minus[i][j].first,
                                             r_minus[i][j].second
                                     });

    for (int j = 0; j < t_num; ++j) {
        for (int i = 0; i < p_num; ++i)
            if (r_mtr[i][j] != 0)
                t_effect[j].push_back({
                                              i,
                                              r_mtr[i][j]
                                      });
        if (t_effect[j].size() > 3) throw invalid_argument("t has more than 3 arrows");
    }

    cpn_t_effect = vector<T_effect>(t_effect.size());
    for (int i = 0; i < t_effect.size(); ++i) {
        for (auto &eff: t_effect[i]) {
            if (q_p[eff.p_index]) {
                cpn_t_effect[i].add_p = eff.p_index;
                cpn_t_effect[i].add_val = eff.num;
            } else if (eff.num > 0) {
                cpn_t_effect[i].push_p = eff.p_index;
            } else {
                cpn_t_effect[i].pop_p = eff.p_index;
            }
        }
    }

    auto t_to_p = vector<vector<int>>(t_num, vector<int>());
    auto p_to_t = vector<vector<int>>(p_num, vector<int>());

    for (int i = 0; i < p_num; ++i)
        for (int j = 0; j < t_num; ++j)
            if (r_plus[i][j] != 0)
                t_to_p[j].push_back(i);

    for (int i = 0; i < p_num; ++i)
        for (int j = 0; j < t_num; ++j)
            if (r_minus[i][j].first != 0)
                p_to_t[i].push_back(j);

    for (int i = 0; i < t_to_p.size(); ++i) {
        for (auto const &p_i: t_to_p[i]) {
            auto ps = p_to_t[p_i];
            for (auto const &it: ps) t_consequences[i].insert(it);
        }
    }

    for (int i = 0; i < t_num; ++i) {
        for (auto const &it: t_consequences[i]) t_consequences[it].insert(i);
    }
}


