#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <string>

#include "group.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

//    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pb_clicked()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::pb_clicked() {
    auto w = new group(this);

    ui->groupsVL->addWidget(w);
}
