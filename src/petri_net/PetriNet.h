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
    int t_i{};
    double gen_time{};
    double sys_time = -1;
};

struct T_Stats {
    int fire_num{};
    vector<double> fire_times{};
    vector<double> gen_times{};

    vector<int> type_fire_num{};
    vector<vector<double>> type_fire_times{};
    vector<vector<double>> type_gen_times{};
};

struct P_Stats {
    int max_chip{};
    int entries{};
    int empty_entries{};
    long long sum{};

    double avg{};

    vector<double> in_time{};
    vector<double> out_time{};

    vector<vector<double>> type_in_time{};
    vector<vector<double>> type_out_time{};
};

struct PetriStatEvent {
    int t;
    int p_out, p_in;
    int type;
    double gen_time;
    double sys_time;
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
    unordered_map<int, int> gen_t;
    unordered_map<int, int> q_info;
    unordered_map<int, unordered_set<int>> selector_t;
    unordered_set<int> win_poc;
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

    unordered_set<int> q_p;
    vector<T_effect> cpn_t_effect;

    vector<deque<int>> m;

    dist_vector timing;

    unordered_map<int, bool> is_wait;

    unordered_set<int> win_poc;
    unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro;

    vector<T_Stats> t_stats;
    vector<P_Stats> p_stats;

    vector<PetriStatEvent> logs;
public:
    PetriNet(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_to_p_arc, int p_num, dist_vector timing,
             vector<int> gen_type, const vector<Q_pos> &q_pos, unordered_map<int, unordered_set<int>> selector_t,
             unordered_set<int> win_poc, unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro);

    void run(int limit);

    vector<PetriEvent> find_fired_t_init();

    vector<PetriEvent> find_fired_t(int t_i);

    void fire_t(PetriEvent event);

    pair<bool, double> is_t_fire(int t_i);

    PetriNetImportData get_import_data();

    pair<vector<T_Stats>, vector<P_Stats>> get_stats();

private:
    pair<bool, double> check_selector_t(int t_i);

    pair<bool, double> check_win_proc_t(int t_i);

    pair<bool, double> check_usual_t(int t_i);

    void process_gen_t(PetriEvent event);

    void process_selector_t(PetriEvent event);

    void process_usual_t(PetriEvent event);

    void push_p_stats(int p_i);

    void count(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc);
};

#endif
