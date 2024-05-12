#ifndef SRC_DELFT_H
#define SRC_DELFT_H

#include <vector>
#include <queue>
#include <memory>

#include "PetriNet.h"

using namespace std;

struct DelftParam {
    int step; // шаг времени нижнего списка
    int mult; // множитель списков
    int m_time; // общее время
};

class Delft {
private:
    DelftParam param; // параметры

    vector<queue<PetriEvent>> low_list; // список 0-го уровня
    int low_pointer; // указатель списка 0-го уровня
    int low_period; // шаг списка 0-го уровня по времени

    vector<vector<int>> top_lists; // верхние списки
    vector<int> level_pointer; // указатели верхних списков
    vector<int> level_period; // шаги верхних списков по времени

    shared_ptr<PetriNet> net; // сеть Петри
public:
    Delft(DelftParam param, shared_ptr<PetriNet> net);

    void run();

    int get_levels_num();

private:
    int execute(int level, int &event_num);

    int execute_zero_lvl(int &event_num);

    int execute_events();

    void insert_events(const vector<PetriEvent> &events, double now);
};

#endif
