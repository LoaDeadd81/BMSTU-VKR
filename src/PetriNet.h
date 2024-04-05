#ifndef SRC_PETRINET_H
#define SRC_PETRINET_H

#include <vector>
#include <exception>

using namespace std;

typedef pair<int, int> Arc; // p t; in/out from p

class PetriNet {
private:
    int p_num, t_num;
    vector<vector<int>> r_minus;
    vector<vector<int>> r_plus;
    vector<vector<int>> r_mtr;
    vector<vector<int>> t_check;
    vector<vector<int>> t_effect;
    vector<int> m;
public:
    PetriNet(int p_num, int t_num, const vector<Arc>& out_arc, const vector<Arc>& in_arc, vector<int> m);

    void run(int limit);

private:
    void count_r(const vector<Arc>& out_arc, const vector<Arc>& in_arc);
    void count_t();

    int find_fired_t();
    bool is_t_fire(int t_i);
    void fire_t(int t_i);
};

#endif
