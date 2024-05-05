#ifndef SRC_PETRINETCONSTRUCTOR_H
#define SRC_PETRINETCONSTRUCTOR_H

#include "PetriNet.h"

struct TypeInfo {
    int type;
    shared_ptr<BaseDistribution> gen_distro;
    shared_ptr<BaseDistribution> proc_distro;
};

class PetriNetConstructor {
public:
    PetriNetConstructor(
            vector<TypeInfo> type_info,
            shared_ptr<BaseDistribution> q_distro,
            vector<int, vector<int>> selectors,
            vector<int> windows_num
    );
};

#endif
