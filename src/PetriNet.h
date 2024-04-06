#ifndef SRC_PETRINET_H
#define SRC_PETRINET_H

#include <vector>
#include <set>
#include <exception>

using namespace std;

typedef pair<int, int> Arc; // p t; in/out from p

struct IngArc {
    Arc arc;
    bool is_ing;
};

struct ArcCheck{
    int p_index;
    int min_num;
    bool is_ing;
};

struct ArcEffect{
    int p_index;
    int num;
};

struct PetriEvent{
    int t_i;
    float time;
};

class PetriNet {
private:
    int p_num, t_num;

    vector<set<int>> t_consequences;
    vector<vector<ArcCheck>> t_check;
    vector<vector<ArcEffect>> t_effect;

    vector<int> m;

    vector<float> timing;
public:
    PetriNet(int p_num, int t_num, const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc, vector<float> timing, vector<int> m);

    void run(int limit);

    vector<PetriEvent>  find_fired_t_init();

    vector<PetriEvent> find_fired_t(int t_i);

    void fire_t(int t_i);

    bool is_t_fire(int t_i);

private:
    void count(const vector<IngArc> &p_to_t_arc, const vector<Arc> &t_po_p_arc);
};

#endif
