#include <iostream>

#include "PetriNet.h"

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

    int p_num = 2, t_num = 1;
    vector<IngArc> out_arc = {
            {{0, 0}, true},
            {{1, 0}, false},
    };
    vector<Arc> in_arc = {
            {0, 1},
    };
    vector<int> m = {0, 1};

//    int p_num = 4, t_num = 2;
//    vector<IngArc> out_arc = {
//            {{0, 0}, false},
//            {{1, 1}, false},
//            {{2, 1}, false},
//    };
//    vector<Arc> in_arc = {
//            {0, 1},
//            {0, 2},
//            {1, 3}
//    };
//    vector<int> m = {1, 0, 0, 0};

    auto p_net = PetriNet(p_num, t_num, out_arc, in_arc, m);
    p_net.run(10);
}
