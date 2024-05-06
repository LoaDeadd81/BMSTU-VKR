#ifndef SRC_PETRINETBUILDER_H
#define SRC_PETRINETBUILDER_H

#include <utility>

#include "PetriNet.h"

struct TypeInfo {
    int type;
    shared_ptr<BaseDistribution> gen_distro;
    shared_ptr<BaseDistribution> proc_distro;
};

struct ReceptionInfo {
    shared_ptr<BaseDistribution> distro;
    int max_q;
};

struct WinGroupInfo {
    int win_num;
    vector<int> types;
    int max_q;
};

struct TimeBlock {
    int in, out;
};

struct QueueStatSource {
    int queue_p;
    TimeBlock block;
    int leave_t;
};

struct WindowQueueStatSource {
    int queue_p;
    int in_t;
    vector<int> out_t;
    int leave_t;
};

struct WindowGroupStatSource {
    vector<int> out_t;
};

struct StatSource {
    int gen_num, out_t;

    QueueStatSource reception_q_src{};
    int reception_src{};

    vector<WindowQueueStatSource> window_q_src;
    vector<WindowGroupStatSource> window_src;
};

class PetriNetBuilder {
private:
    vector<TypeInfo> type_info;
    ReceptionInfo rec_info;
    vector<WinGroupInfo> win_info;

    //general
    vector<IngArc> p_to_t_arc;
    vector<Arc> t_to_p_arc;
    int p_num{};
    dist_vector timing;

    //gen
    vector<int> gen_type;

    //reception and group
    vector<Q_pos> q_pos;

    //group
    unordered_map<int, unordered_set<int>> selector_t;
    //windows
    unordered_set<int> win_poc;
    //types
    unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro;

    StatSource src;

public:
    PetriNetBuilder(
            vector<TypeInfo> type_info,
            ReceptionInfo rec_info,
            vector<WinGroupInfo> win_info
    ) : type_info(std::move(type_info)), rec_info(std::move(rec_info)), win_info(std::move(win_info)) {};

    shared_ptr<PetriNet> build();

    StatSource get_stat_src();

private:
    void add_types(const vector<TypeInfo> &info);

    void add_gens(const vector<TypeInfo> &info);

    void add_reception(const ReceptionInfo &info);

    void add_groups(const vector<WinGroupInfo> &info);

    void add_win_groups(const WinGroupInfo &info, int select_from);

    TimeBlock add_win(const WinGroupInfo &info, int select_from, int q_p);
};

#endif
