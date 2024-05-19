#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto rw = new ReceptionWidget();
    ui->receptionVL->addWidget(rw);

    QObject::connect(ui->groupPlusPB, SIGNAL(clicked()), this, SLOT(plus_group_clicked()));
    QObject::connect(ui->groupMinusPB, SIGNAL(clicked()), this, SLOT(minus_group_clicked()));
    QObject::connect(ui->windowsPlusPB, SIGNAL(clicked()), this, SLOT(plus_windows_clicked()));
    QObject::connect(ui->windowsMinusPB, SIGNAL(clicked()), this, SLOT(minus_windows_clicked()));
    QObject::connect(ui->countPB, SIGNAL(clicked()), this, SLOT(count()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::plus_group_clicked() {
    auto gw = new GroupWidget(ui->groupsVL->count() + 1, this);

    for (int i = 0; i < ui->windowsVL->count(); ++i) {
        auto ww = dynamic_cast<WindowsWidget *>(ui->windowsVL->itemAt(i)->widget());
        ww->add_group();
    }

    ui->groupsVL->addWidget(gw);
}

void MainWindow::minus_group_clicked() {
    auto gw = ui->groupsVL->itemAt(ui->groupsVL->count() - 1);
    if (gw != nullptr) delete gw->widget();

    for (int i = 0; i < ui->windowsVL->count(); ++i) {
        auto ww = dynamic_cast<WindowsWidget *>(ui->windowsVL->itemAt(i)->widget());
        ww->del_group();
    }
}

void MainWindow::plus_windows_clicked() {
    auto ww = new WindowsWidget(ui->groupsVL->count(), this);

    ui->windowsVL->addWidget(ww);
}

void MainWindow::minus_windows_clicked() {
    auto ww = ui->windowsVL->itemAt(ui->windowsVL->count() - 1);
    if (ww != nullptr) delete ww->widget();
}

void MainWindow::count() {
    vector<TypeInfo> type_info;
    ReceptionInfo rec_info;
    vector<WinGroupInfo> win_info;

    for (int i = 0; i < ui->groupsVL->count(); ++i) {
        auto gw = dynamic_cast<GroupWidget *>(ui->groupsVL->itemAt(i)->widget());
        type_info.push_back(gw->get_type_info());
    }

    auto rw = dynamic_cast<ReceptionWidget *>(ui->receptionVL->itemAt(0)->widget());
    rec_info = rw->get_rec_info();

    for (int i = 0; i < ui->windowsVL->count(); ++i) {
        auto ww = dynamic_cast<WindowsWidget *>(ui->windowsVL->itemAt(i)->widget());
        win_info.push_back(ww->get_win_group_info());
    }

    int mtime = ui->mtimeSB->value();

    auto builder = PetriNetBuilder(type_info, rec_info, win_info);
    auto p_net = builder.build();
    auto src = builder.get_stat_src();

    auto drawer = PetriNetDrawer(p_net->get_import_data());
    drawer.draw("net.svg");

    DelftParam param(1, 5, mtime);
    auto delft = Delft(param, p_net);
    delft.run();

    auto stats = p_net->get_stats();
    auto aggregator = StatsAggregator(mtime, src, stats.first, stats.second);
    auto smoStats = aggregator.get_stats();

    auto result = new ResultWindow(smoStats, this);
    result->show();

    auto logs = p_net->get_logs_per_transact();
    auto anim = new AnimationWindow(src, logs, param.m_time, this);
    anim->show();
}
