#ifndef GROUP_H
#define GROUP_H

#include <QWidget>
#include <vector>
#include <stdexcept>

#include "ui_group.h"
#include "DistributionsCFG.h"
#include "PetriNetBuilder.h"

using namespace std;

class GroupWidget : public QWidget {
Q_OBJECT
public:
    explicit GroupWidget(int num, QWidget *parent = nullptr);

    TypeInfo get_type_info();

private:
    Ui::Group *ui;

    int num;
};

#endif
