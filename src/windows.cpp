#include "windows.h"

WindowsWidget::WindowsWidget(int group_num, QWidget *parent)
        : QWidget{parent}, ui(new Ui::Windows) {
    ui->setupUi(this);

    for (int i = 0; i < group_num; ++i) {
        auto item = new QListWidgetItem(QString("%1").arg(i + 1));
        item->setCheckState(Qt::Unchecked);
        ui->typesLW->addItem(item);
    }

}

WinGroupInfo WindowsWidget::get_win_group_info() {
    vector<int> groups;

    for (int i = 0; i < ui->typesLW->count(); ++i)
        if (ui->typesLW->item(i)->checkState() == Qt::Checked)
            groups.push_back(i + 1);

    return WinGroupInfo{
            ui->winNumSB->value(),
            groups,
            ui->maxQSB->value()
    };
}

void WindowsWidget::add_group() {
    auto item = new QListWidgetItem(QString("%1").arg(ui->typesLW->count() + 1));
    item->setCheckState(Qt::Unchecked);
    ui->typesLW->addItem(item);
}

void WindowsWidget::del_group() {
    ui->typesLW->takeItem(ui->typesLW->count() - 1);
}
