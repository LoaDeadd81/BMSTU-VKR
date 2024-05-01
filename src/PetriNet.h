#ifndef SRC_PETRINET_H
#define SRC_PETRINET_H

#include <vector>
#include <set>
#include <exception>
#include <memory>
#include <queue>
#include <unordered_map>

#include "Distributions.h"

using namespace std;

typedef pair<int, int> Arc; // p t; in/out from p

typedef vector<shared_ptr<BaseDistribution>> dist_vector;

struct IngArc {
    Arc arc;
    bool is_ing;
};

struct ArcCheck {
    int p_index;
    int min_num;
    bool is_ing;
};

struct ArcEffect {
    int p_index;
    int num;
};

struct PetriEvent {
    int t_i;
    double time;
};

struct T_Stats{
    int fire_num;
    long double work_time;
};

struct P_Stats{
    int entries;
    int max_chip;
    queue<double> in_time;
    queue<double> out_time;
};

class PetriNet {
private:
    int p_num, t_num;

    vector<set<int>> t_consequences;
    vector<vector<ArcCheck>> t_check;
    vector<vector<ArcEffect>> t_effect;

    vector<int> m;

    dist_vector timing;

    unordered_map<int, bool> is_wait;

    vector<T_Stats> t_stat;
    vector<P_Stats> p_stat;
public:
    PetriNet(int p_num, int t_num, const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc,vector<int> m, dist_vector timing);

    void run(int limit);

    vector<PetriEvent> find_fired_t_init();

    vector<PetriEvent> find_fired_t(int t_i);

    void fire_t(PetriEvent event);

    void process_p(int p_i, int chip_num, double time);

    bool is_t_fire(int t_i);

private:
    void count(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc);
};

#endif
