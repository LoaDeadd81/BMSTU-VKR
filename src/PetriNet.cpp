#include "PetriNet.h"

#include <utility>
#include <iostream>

#include "Error.h"

PetriNet::PetriNet(int p_num, int t_num, const vector<Arc>& out_arc, const vector<Arc>& in_arc, vector<int> m) {
    this->p_num = p_num;
    this->t_num = t_num;

    r_minus = vector<vector<int>>(p_num, vector<int>(t_num, 0));
    r_plus = vector<vector<int>>(p_num, vector<int>(t_num, 0));
    r_mtr = vector<vector<int>>(p_num, vector<int>(t_num, 0));
    count_r(out_arc, in_arc);

    t_check = vector<vector<int>>(t_num, vector<int>(p_num, 0));
    t_effect = vector<vector<int>>(t_num, vector<int>(p_num, 0));
    count_t();

    if (m.size() != p_num) throw Error("m len != p_num");
    this->m = std::move(m);
}

void PetriNet::run(int limit) {
    int count = 0;

    while (count < limit){
        int t_to_fire = find_fired_t();
        if (t_to_fire < 0) break;

        fire_t(t_to_fire);
        cout << "fired: " << t_to_fire << endl;
        cout << "M: ";
        for (auto const &it: m) cout << " " << it;
        cout << endl;

        count++;
    }
}

void PetriNet::count_r(const vector<Arc>& out_arc, const vector<Arc>& in_arc) {
    for (const auto &it: out_arc)
        r_minus[it.first][it.second] = 1;

    for (const auto &it: in_arc)
        r_plus[it.first][it.second] = 1;

    for (int i = 0; i < p_num; ++i)
        for (int j = 0; j < t_num; ++j)
            r_mtr[i][j] = r_plus[i][j] - r_minus[i][j];
}

void PetriNet::count_t() {
    for (int j = 0; j < t_num; ++j)
        for (int i = 0; i < p_num; ++i)
            if (r_minus[i][j] != 0)
                t_check[j][i] = r_minus[i][j];

    for (int j = 0; j < t_num; ++j)
        for (int i = 0; i < p_num; ++i)
            if (r_mtr[i][j] != 0)
                t_effect[j][i] = r_mtr[i][j];
}

int PetriNet::find_fired_t() {
    for (int i = 0; i < t_num; ++i)
        if (is_t_fire(i))
            return i;

    return -1;
}

bool PetriNet::is_t_fire(int t_i){
    for (int i = 0; i < p_num; ++i)
        if(m[i] < t_check[t_i][i])
            return false;

    return true;
}

void PetriNet::fire_t(int t_i) {
    for (int i = 0; i < p_num; ++i)
        m[i] += t_effect[t_i][i];
}


