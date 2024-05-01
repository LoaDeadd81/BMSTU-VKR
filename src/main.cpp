#include <iostream>
#include <random>

#include "PetriNet.h"
#include "Delft.h"

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

    int p_num = 6, t_num = 8;
    vector<IngArc> out_arc = {
            {{0, 2}, false},
            {{0, 6}, false},
            {{1, 3}, false},
            {{2, 4}, false},
            {{2, 7}, false},
            {{3, 5}, false},
            {{4, 2}, false},
            {{4, 6}, true},
            {{5, 4}, false},
            {{5, 7}, true},
    };
    vector<Arc> in_arc = {
            {0, 0},
            {1, 0},
            {2, 1},
            {3, 2},
            {3, 4},
            {4, 3},
            {5, 5},
    };
    vector<int> m = {0, 0, 0, 0, 5, 10};
    dist_vector timing = {
            make_shared<ConstDistribution>(5),
            make_shared<ConstDistribution>(10),
            make_shared<ImmediateDistribution>(),
            make_shared<ConstDistribution>(10),
            make_shared<ImmediateDistribution>(),
            make_shared<ConstDistribution>(20),
            make_shared<ImmediateDistribution>(),
            make_shared<ImmediateDistribution>(),
    };

    auto p_net = make_shared<PetriNet>(p_num, t_num, out_arc, in_arc, m, timing);
//    p_net->run(100000);
//    cout << "";
    DelftParam param(1, 5, 210);
    auto delft = Delft(param, p_net);
    delft.run();
    cout << "";
}
