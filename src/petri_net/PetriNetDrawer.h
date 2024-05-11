#ifndef SRC_PETRINETDRAWER_H
#define SRC_PETRINETDRAWER_H

#include <string>
#include <utility>

#include "PetriNet.h"

using namespace std;

class PetriNetDrawer {
private:
    PetriNetImportData data;
public:
    explicit PetriNetDrawer(PetriNetImportData data) : data(std::move(data)) {};

    void draw(const string &filename);
};

#endif
