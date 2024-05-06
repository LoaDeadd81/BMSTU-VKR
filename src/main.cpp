#include <iostream>
#include <random>

#include "PetriNet.h"
#include "Delft.h"
#include "PetriNetDrawer.h"
#include "PetriNetBuilder.h"

using namespace std;

int main() {

//    int p_num = 5, t_num = 4;
//    vector<IngArc> out_arc = {
//            {{0, 1}, false},
//            {{1, 1}, false},
//            {{2, 3}, false},
//            {{3, 0}, false},
//            {{4, 2}, false},
//    };ww
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 2},
//            {2, 1},
//            {3, 3},
//            {3, 4},
//    };
//    vector<int> m = {1, 1, 0, 0, 0};

//    int p_num = 3, t_num = 4;
//    vector<IngArc> out_arc = {
//            {{0, 3}, false},
//            {{0, 1}, false},
//            {{1, 2}, false},
//            {{2, 1}, false},
//            {{2, 3}, true},
//    };
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 1},
//            {2, 2},
//    };
//    vector<int> m = {0, 0, 5};
//    dist_vector timing = {
//            make_shared<ConstDistribution>(5),
//            make_shared<ImmediateDistribution>(),
//            make_shared<ConstDistribution>(20),
//            make_shared<ImmediateDistribution>()
//    };

//    int p_num = 6, t_num = 8;
//    vector<IngArc> out_arc = {
//            {{0, 2}, false},
//            {{0, 6}, false},
//            {{1, 3}, false},
//            {{2, 4}, false},
//            {{2, 7}, false},
//            {{3, 5}, false},
//            {{4, 2}, false},
//            {{4, 6}, true},
//            {{5, 4}, false},
//            {{5, 7}, true},
//    };
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 0},
//            {2, 1},
//            {3, 2},
//            {3, 4},
//            {4, 3},
//            {5, 5},
//    };
//    dist_vector timing = {
//            make_shared<ConstDistribution>(5),
//            make_shared<ConstDistribution>(10),
//            make_shared<ImmediateDistribution>(),
//            make_shared<ConstDistribution>(10),
//            make_shared<ImmediateDistribution>(),
//            make_shared<ConstDistribution>(20),
//            make_shared<ImmediateDistribution>(),
//            make_shared<ImmediateDistribution>(),
//    };
//    vector<int> gen_type = {1, 1};
//    vector<Q_pos> q_pos = {
//            {4, 5},
//            {5, 10}
//    };
//    unordered_map<int, unordered_set<int>> selector_t = {
//};

//    int p_num = 1, t_num = 4;
//    vector<IngArc> out_arc = {
//            {{0, 2}, false},
//            {{0, 3}, false},
//    };
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 0},
//    };
//    dist_vector timing = {
//            make_shared<ConstDistribution>(2),
//            make_shared<ConstDistribution>(4),
//            make_shared<ConstDistribution>(4),
//            make_shared<ConstDistribution>(8),
//    };
//    vector<int> gen_type = {1, 2};
//    vector<Q_pos> q_pos = {};
//    unordered_map<int, unordered_set<int>> selector_t = {
//            {2, {1}},
//            {3, {2}}
//    };

    int p_num = 3, t_num = 7;
    vector<IngArc> out_arc = {
            {{0, 3}, false},
            {{0, 5}, false},
            {{1, 4}, false},
            {{2, 6}, false},
    };
    vector<Arc> in_arc = {
            {0, 0},
            {1, 0},
            {2, 0},
            {3, 1},
            {5, 2},
    };
    dist_vector timing = {
            make_shared<ConstDistribution>(1),
            make_shared<ConstDistribution>(2),
            make_shared<ConstDistribution>(2),
            make_shared<ImmediateDistribution>(),
            make_shared<ConstDistribution>(10),
            make_shared<ImmediateDistribution>(),
            make_shared<ConstDistribution>(20),
    };
    vector<int> gen_type = {1, 2, 3};
    vector<Q_pos> q_pos = {};
    unordered_map<int, unordered_set<int>> selector_t = {
            {3, {1, 2}},
            {5, {3}}
    };
    unordered_set<int> win_poc = {4, 6};
    unordered_map<int, shared_ptr<BaseDistribution>> type_proc_distro = {
            {1, make_shared<ConstDistribution>(10)},
            {2, make_shared<ConstDistribution>(10)},
            {3, make_shared<ConstDistribution>(20)},
    };

    auto p_net = make_shared<PetriNet>(out_arc, in_arc, p_num, timing, gen_type, q_pos, selector_t, win_poc,
                                       type_proc_distro);

//    vector<TypeInfo> type_info = {
//            {1, make_shared<ConstDistribution>(1), make_shared<ConstDistribution>(1)},
//            {2, make_shared<ConstDistribution>(1), make_shared<ConstDistribution>(1)},
//            {3, make_shared<ConstDistribution>(1), make_shared<ConstDistribution>(1)},
//    };
//    ReceptionInfo rec_info = {make_shared<ConstDistribution>(1), 10};
//    vector<WinGroupInfo> win_info = {
//            {2, {1}, 10},
//            {2, {2}, 10},
//            {2, {3}, 10},
//    };
//
//    auto builder = PetriNetBuilder(type_info, rec_info, win_info);
//    auto p_net = builder.build();
//    auto src = builder.get_stat_src();

    auto drawer = PetriNetDrawer(p_net->get_import_data());
    drawer.draw("net.svg");

    DelftParam param(1, 5, 400);
    auto delft = Delft(param, p_net);
    delft.run();
    cout << "";
}
