#ifndef SRC_DELFT_H
#define SRC_DELFT_H

#include <vector>
#include <queue>
#include <memory>

#include "PetriNet.h"

using namespace std;

struct DelftParam {
    int step;
    int mult;
    int m_time;
};

class Delft {
private:
    DelftParam param;

    vector<queue<PetriEvent>> low_list;
    int low_pointer;
    int low_period;

    vector<vector<int>> top_lists;
    vector<int> level_pointer;
    vector<int> level_period;

    shared_ptr<PetriNet> net;
public:
    Delft(DelftParam param, shared_ptr<PetriNet> net);

    void run();

private:
    int execute(int level, int &event_num);
    int execute_zero_lvl(int &event_num);
    int execute_events();

    void insert_events(const vector<PetriEvent> &events, double now);
};

#endif
