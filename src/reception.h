#ifndef RECEPTIONWIDGET_H
#define RECEPTIONWIDGET_H

#include <QWidget>

#include "ui_reception.h"
#include "PetriNetBuilder.h"
#include "DistributionsCFG.h"


class ReceptionWidget : public QWidget {
Q_OBJECT
public:
    explicit ReceptionWidget(QWidget *parent = nullptr);

    ReceptionInfo get_rec_info();

private:
    Ui::Reception *ui;
};

#endif
