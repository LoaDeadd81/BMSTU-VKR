#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QMessageBox>

#include "PetriNetDrawer.h"
#include "Delft.h"
#include "Stats.h"

#include "group.h"
#include "reception.h"
#include "windows.h"
#include "result.h"
#include "animation.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    shared_ptr<PetriNet> p_net;
    StatSource src;
    int mtime;

private slots:

    void plus_group_clicked();

    void minus_group_clicked();

    void plus_windows_clicked();

    void minus_windows_clicked();

    void count();

    void animate();
};

#endif // MAINWINDOW_H
