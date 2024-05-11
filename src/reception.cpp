#include "reception.h"

ReceptionWidget::ReceptionWidget(QWidget *parent)
        : QWidget{parent}, ui(new Ui::Reception) {
    ui->setupUi(this);

    init_SB(ui->procIntCB);
}

ReceptionInfo ReceptionWidget::get_rec_info() {
    return ReceptionInfo{
            get_distro_by_id(ui->procIntCB->currentIndex(), ui->procMinSB->value(), ui->procMaxSB->value()),
            ui->maxQSB->value()
    };
}
