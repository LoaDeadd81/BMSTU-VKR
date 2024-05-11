#include "group.h"

group::group(QWidget *parent)
    : QWidget{parent}, ui(new Ui::Group)
{
    ui->setupUi(this);
}
