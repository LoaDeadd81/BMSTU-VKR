#ifndef SRC_PETRINET_H
#define SRC_PETRINET_H

#include <vector>
#include <set>
#include <exception>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

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

struct T_Stats {
    int fire_num;
    long double work_time;
};

struct P_Stats {
    int entries{};
    int max_chip{};
    queue<double> in_time;
    queue<double> out_time;
};

struct T_effect {
    int pop_p, push_p;
    int add_p, add_val;

    T_effect() : pop_p(-1), push_p(-1), add_p(-1), add_val(0) {};
};

struct Q_pos {
    int p_i;
    int val;
};

struct PetriNetImportData {
    int p_num, t_num;
    vector<IngArc> p_to_t_arc;
    vector<Arc> t_to_p_arc;
};

class PetriNet {
private:
    int p_num, t_num;
    vector<IngArc> p_to_t_arc;
    vector<Arc> t_to_p_arc;

    vector<set<int>> t_consequences;
    vector<vector<ArcCheck>> t_check;
    vector<vector<ArcEffect>> t_effect;

    unordered_map<int, int> gen_t;
    unordered_map<int, unordered_set<int>> selector_t;

    unordered_map<int, bool> q_p;
    vector<T_effect> cpn_t_effect;

    vector<deque<int>> m;

    dist_vector timing;

    unordered_map<int, bool> is_wait;

    unordered_set<int> win_poc;
    unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro;

    vector<T_Stats> t_stat;
    vector<P_Stats> p_stat;
public:
    PetriNet(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc, int p_num, dist_vector timing,
             vector<int> gen_type, const vector<Q_pos> &q_pos, unordered_map<int, unordered_set<int>> selector_t,
             unordered_set<int> win_poc, unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro);

    void run(int limit);

    vector<PetriEvent> find_fired_t_init();

    vector<PetriEvent> find_fired_t(int t_i);

    void fire_t(PetriEvent event);

    pair<bool, double> is_t_fire(int t_i);

    PetriNetImportData get_import_data();

private:
    pair<bool, double> check_selector_t(int t_i);

    pair<bool, double> check_win_proc_t(int t_i);

    pair<bool, double> check_usual_t(int t_i);

    void process_gen_t(int t_i);

    void process_selector_t(int t_i);

    void process_usual_t(int t_i);

    void count(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc);
};

#endif
