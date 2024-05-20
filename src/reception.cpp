#include "reception.h"

#define EPS 0.1

ReceptionWidget::ReceptionWidget(QWidget *parent)
        : QWidget{parent}, ui(new Ui::Reception) {
    ui->setupUi(this);

    init_SB(ui->procIntCB);
}

ReceptionInfo ReceptionWidget::get_rec_info() {
    if ((ui->procMaxSB->value() - ui->procMinSB->value()) < EPS)
        throw runtime_error(
                "Максимальное значение времени обработки заявки ресепшеном должно быть больше минимального минимума чем на 0.1");

    return ReceptionInfo{
            get_distro_by_id(ui->procIntCB->currentIndex(), ui->procMinSB->value(), ui->procMaxSB->value()),
            ui->maxQSB->value()
    };
}
