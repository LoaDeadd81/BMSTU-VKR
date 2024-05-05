#include "Delft.h"

#include <utility>
#include <iostream>

#define EPS 1e-4

Delft::Delft(DelftParam param, shared_ptr<PetriNet> net) {
    this->param = param;

    this->net = std::move(net);

    low_list = vector<queue<PetriEvent>>(param.m_time / param.step + 1);
    low_period = param.step;
    low_pointer = 0;

    int time_period = param.step * param.mult;
    while (time_period < param.m_time / 2) {
        level_period.push_back(time_period);
        top_lists.emplace_back(param.m_time / time_period + 1);
        time_period *= param.mult;
    }

    level_pointer = vector<int>(top_lists.size(), 0);
}

void Delft::run() {
    insert_events(net->find_fired_t_init(), 0);

    int level = top_lists.size() - 1;
    while (true) {
        while (level_pointer[level] < top_lists[level].size() && top_lists[level][level_pointer[level]] == 0)
            level_pointer[level]++;
        if (level_pointer[level] == top_lists[level].size() && top_lists[level][level_pointer[level] - 1] == 0)
            break;

        if (level - 1 < 0) {
            low_pointer = level_pointer[level] * param.mult;
            execute_zero_lvl(top_lists[level][level_pointer[level]]);
        } else {
            level_pointer[level - 1] = level_pointer[level] * param.mult;
            execute(level - 1, top_lists[level][level_pointer[level]]);
        }
//        level_pointer[level - 1] = level_pointer[level] * param.mult;
//        execute(level - 1, top_lists[level][level_pointer[level]]);

        level_pointer[level]++;
    }
}

int Delft::execute(int level, int &event_num) {
    int event_done = 0;

    while (event_num > 0) {
        while (level_pointer[level] < top_lists[level].size() && top_lists[level][level_pointer[level]] == 0)
            level_pointer[level]++;
//        if (level_pointer[level] == top_lists[level].size() && top_lists[level][level_pointer[level] - 1] == 0)
//            break;

        int exc = 0;
        if (level - 1 < 0) {
            low_pointer = level_pointer[level] * param.mult;
            exc = execute_zero_lvl(top_lists[level][level_pointer[level]]);
        } else {
            level_pointer[level - 1] = level_pointer[level] * param.mult;
            exc = execute(level - 1, top_lists[level][level_pointer[level]]);
        }

        event_num -= exc;
        event_done += exc;

        level_pointer[level]++;
    }

    return event_done;
}

int Delft::execute_zero_lvl(int &event_num) {
    int event_done = 0;

    while (event_num > 0) {
        while (low_pointer < low_list.size() && low_list[low_pointer].empty())
            low_pointer++;
//        if (low_pointer < low_list.size() && top_lists[level][level_pointer[level] - 1] == 0)
//            break;

        int exc = execute_events();

        event_num -= exc;
        event_done += exc;

        low_pointer++;
    }

    return event_done;
}

int Delft::execute_events() {
    int event_done = 0;
    auto &q = low_list[low_pointer];

    while (!q.empty()) {
        event_done++;

        auto event = q.front();
        q.pop();
        auto check_res = net->is_t_fire(event.t_i);
        if (!check_res.first) continue;

        cout << "time: " << event.time << endl;
        net->fire_t(event);
        insert_events(net->find_fired_t_init(), event.time);
    }

    return event_done;
}

void Delft::insert_events(const vector<PetriEvent> &events, double now) {
    for (auto it: events) {
        if (it.time == 0) {
            it.time = now;
            for (int i = 0; i < top_lists.size(); ++i) top_lists[i][level_pointer[i]]++;
            low_list[low_pointer].push(it);
        } else {
            double time = now + it.time;
            if (time > param.m_time)continue;

            it.time = time;
            time -= EPS;

            for (int i = 0; i < top_lists.size(); ++i) {
                int index = int(time / level_period[i]);
                top_lists[i][index]++;
            }

            low_list[int(time / low_period)].push(it);
        }
    }
}
