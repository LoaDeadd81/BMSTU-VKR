#include "PetriNet.h"

#include <utility>
#include <iostream>

#include "Error.h"

PetriNet::PetriNet(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc, int p_num,
                   const vector<Q_pos> &q_pos,
                   dist_vector timing, vector<int> gen_type) {
    this->p_num = p_num;
    t_num = timing.size();

    for (int i = 0; i < gen_type.size(); ++i) gen_t[i] = gen_type[i];

    m = vector<queue<int>>(p_num);
    for (auto pos: q_pos) {
        q_p[pos.p_i] = true;
        for (int i = 0; i < pos.val; ++i)
            m[pos.p_i].push(-1);
    }

    t_consequences = vector<set<int>>(t_num, set<int>());
    t_check = vector<vector<ArcCheck>>(t_num, vector<ArcCheck>());
    t_effect = vector<vector<ArcEffect>>(t_num, vector<ArcEffect>());
    count(p_to_t_arc, t_po_p_arc);

    this->timing = std::move(timing);

    for (int i = 0; i < t_num; ++i) {
        is_wait[i] = false;
    }

    p_stat = vector<P_Stats>(p_num);
    t_stat = vector<T_Stats>(t_num);
}

void PetriNet::run(int limit) {
    int count = 0;
    queue<PetriEvent> q;
    auto to_fire = find_fired_t_init();
    for (auto it: to_fire) q.push(it);

    while (!q.empty() && count < limit) {
        auto event = q.back();
        q.pop();

        if (!is_t_fire(event.t_i)) continue;

        fire_t(event);
        to_fire = find_fired_t(event.t_i);
        for (auto it: to_fire) q.push(it);

        count++;
    }
}

vector<PetriEvent> PetriNet::find_fired_t_init() {
    vector<PetriEvent> res;

    for (int i = 0; i < t_num; ++i)
        if (!is_wait[i] && is_t_fire(i)) {
            is_wait[i] = true;
            double gen = timing[i]->gen();
            res.push_back({i, gen});
            cout << "to fire: " << i << " int: " << gen << endl;
        }

    return res;
}

vector<PetriEvent> PetriNet::find_fired_t(int t_i) {
    vector<PetriEvent> res;

    for (auto const &i: t_consequences[t_i])
        if (!is_wait[i] && is_t_fire(i)) {
            is_wait[i] = true;
            double gen = timing[i]->gen();
            res.push_back({i, gen});
            cout << "to fire: " << i << " int: " << gen << endl;
        }

    return res;
}

bool PetriNet::is_t_fire(int t_i) {
    for (auto const &it: t_check[t_i])
        if (m[it.p_index].size() < it.min_num || !m[it.p_index].empty() && it.is_ing)
            return false;

    return true;
}

void PetriNet::fire_t(PetriEvent event) {
    t_stat[event.t_i].fire_num++;

    is_wait[event.t_i] = false;
//    for (auto const &it: t_effect[event.t_i])
//        process_p(it.p_index, it.num, event.time);
    auto eff = cpn_t_effect[event.t_i];
    if(gen_t.contains(event.t_i)){
        m[eff.push_p].push(gen_t[event.t_i]);
    } else {
        if (eff.add_p >= 0) {
            if (eff.add_val > 0) m[eff.add_p].push(-1);
            else m[eff.add_p].pop();
        }

        int val = m[eff.pop_p].front();
        m[eff.pop_p].pop();
        if(eff.push_p >= 0) m[eff.push_p].push(val);
    }


    cout << endl << endl;
    cout << "fired: " << event.t_i << endl;
    cout << "M: ";
    for (auto it: m) cout << it.size() << " ";
    cout << endl;

}

//void PetriNet::process_p(int p_i, int chip_num, double time) {
//    m[p_i] += chip_num;
//
//    if (chip_num > 0) {
//        p_stat[p_i].entries += chip_num;
//        //stats
//        if (m[p_i] > p_stat[p_i].max_chip) p_stat[p_i].max_chip = m[p_i];
//        for (int i = 0; i < chip_num; ++i) p_stat[p_i].in_time.push(time);
//    } else {
//        //stats
//        for (int i = 0; i < chip_num * -1; ++i) p_stat[p_i].out_time.push(time);
//    }
//}

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


