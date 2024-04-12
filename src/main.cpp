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
//    };
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 2},
//            {2, 1},
//            {3, 3},
//            {3, 4},
//    };
//    vector<int> m = {1, 1, 0, 0, 0};

    int p_num = 3, t_num = 2;
    vector<IngArc> out_arc = {
            {{0, 0}, false},
            {{1, 1}, false},
            {{1, 0}, true},
    };
    vector<Arc> in_arc = {
            {0, 1},
            {1, 2},
    };
    vector<int> m = {100, 0, 0};
    dist_vector timing = {
            make_shared<ImmediateDistribution>(),
            make_shared<UniformDistribution>(1, 3)
    };

    auto p_net = make_shared<PetriNet>(p_num, t_num, out_arc, in_arc, m, timing);
//    p_net->run(30);
//    cout << "";
    DelftParam param(1, 5, 300);
    auto delft = Delft(param, p_net);
    delft.run();
}
