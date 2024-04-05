#include <iostream>

#include "PetriNet.h"

using namespace std;

int main() {
//    int p_num = 5, t_num = 4;
//    vector<Arc> out_arc = {
//            {0, 1},
//            {1, 1},
//            {1, 2},
//            {2, 3},
//            {3, 0},
//    };
//    vector<Arc> in_arc = {
//            {0, 0},
//            {1, 2},
//            {2, 1},
//            {3, 3},
//            {4, 2},
//            {4, 3},
//    };
//    vector<int> m = {1, 1, 0, 0, 0};

    int p_num = 5, t_num = 4;
    vector<Arc> out_arc = {
            {0, 1},
            {1, 1},
            {2, 3},
            {3, 0},
            {4, 2}
    };
    vector<Arc> in_arc = {
            {0, 0},
            {1, 2},
            {2, 1},
            {3, 3},
            {4, 3},
    };
    vector<int> m = {1, 1, 0, 0, 0};

    auto p_net = PetriNet(p_num, t_num, out_arc, in_arc, m);
    p_net.run(10);
}
