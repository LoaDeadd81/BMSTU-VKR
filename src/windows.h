#ifndef WINDOWSWIDGET_H
#define WINDOWSWIDGET_H

#include <QWidget>

#include "ui_windows.h"
#include "PetriNetBuilder.h"

class WindowsWidget : public QWidget {
Q_OBJECT
public:
    explicit WindowsWidget(int group_num, QWidget *parent = nullptr);

    void add_group();

    void del_group();

    WinGroupInfo get_win_group_info();

private:
    Ui::Windows *ui;
};

#endif
