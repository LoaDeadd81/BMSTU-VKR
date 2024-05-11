#include "group.h"

GroupWidget::GroupWidget(int num, QWidget *parent)
        : QWidget{parent}, ui(new Ui::Group), num(num) {
    ui->setupUi(this);

    ui->label->setText(QString("Группа %1").arg(num));
    init_SB(ui->arrivalIntCB);
    init_SB(ui->procIntCB);
}

TypeInfo GroupWidget::get_type_info() {
    return TypeInfo{
            num,
            get_distro_by_id(ui->arrivalIntCB->currentIndex(), ui->arrMinSB->value(), ui->arrMaxSB->value()),
            get_distro_by_id(ui->procIntCB->currentIndex(), ui->procMinSB->value(), ui->procMaxSB->value()),
    };
}
