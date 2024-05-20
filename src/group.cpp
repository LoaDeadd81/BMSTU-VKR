#include "group.h"

#define EPS 0.1

GroupWidget::GroupWidget(int num, QWidget *parent)
        : QWidget{parent}, ui(new Ui::Group), num(num) {
    ui->setupUi(this);

    ui->label->setText(QString("Группа %1").arg(num));
    init_SB(ui->arrivalIntCB);
    init_SB(ui->procIntCB);
}

TypeInfo GroupWidget::get_type_info() {
    if ((ui->arrMaxSB->value() - ui->arrMinSB->value()) < EPS)
        throw runtime_error(
                "Максимальное значение времени прихода должно быть больше минимального минимума чем на 0.1");
    if ((ui->procMaxSB->value() - ui->procMinSB->value()) < EPS)
        throw runtime_error(
                "Максимальное значение времени обработки заявки должно быть больше минимального минимума чем на 0.1");

    return TypeInfo{
            num,
            get_distro_by_id(ui->arrivalIntCB->currentIndex(), ui->arrMinSB->value(), ui->arrMaxSB->value()),
            get_distro_by_id(ui->procIntCB->currentIndex(), ui->procMinSB->value(), ui->procMaxSB->value()),
    };
}
