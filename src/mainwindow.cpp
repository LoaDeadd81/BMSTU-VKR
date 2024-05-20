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
    QObject::connect(ui->showPB, SIGNAL(clicked()), this, SLOT(animate()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::plus_group_clicked() {
    try {
        if (ui->groupsVL->count() >= 8)
            throw runtime_error("Нельзя добавить больше 8 типов заявок");


        auto gw = new GroupWidget(ui->groupsVL->count() + 1, this);

        for (int i = 0; i < ui->windowsVL->count(); ++i) {
            auto ww = dynamic_cast<WindowsWidget *>(ui->windowsVL->itemAt(i)->widget());
            ww->add_group();
        }

        ui->groupsVL->addWidget(gw);
    } catch (const exception &e) {
        auto msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Ошибка");
        msg->setInformativeText(e.what());
        msg->setWindowTitle("Ошибка");
        msg->exec();
    }
}

void MainWindow::minus_group_clicked() {
    if (ui->groupsVL->isEmpty()) return;

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
    if (ui->windowsVL->isEmpty()) return;

    auto ww = ui->windowsVL->itemAt(ui->windowsVL->count() - 1);
    if (ww != nullptr) delete ww->widget();
}

void MainWindow::count() {
    try {
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

        mtime = ui->mtimeSB->value();

        if (type_info.empty())
            throw runtime_error("Необходимо добавть хотя бы 1 группу заявок");
        if (win_info.empty())
            throw runtime_error("Необходимо добавть хотя бы 1 группу окон");

        if (type_info.size() > 8)
            throw runtime_error("Нельзя добавить больше 8 типов заявок");

        set<int> type_set;
        for (auto &win: win_info) {
            for (auto &type: win.types) {
                if (type_set.contains(type))
                    throw runtime_error("Несколько групп окон не могут обрабатывать один и тот же тип заявок");
                type_set.insert(type);
            }
        }
        if (type_set.size() != type_info.size())
            throw runtime_error("Не все группы заявок выбраны для выполнения");


        auto builder = PetriNetBuilder(type_info, rec_info, win_info);
        p_net = builder.build();
        src = builder.get_stat_src();

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
        
    } catch (const exception &e) {
        auto msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Ошибка");
        msg->setInformativeText(e.what());
        msg->setWindowTitle("Ошибка");
        msg->exec();
    }
}

void MainWindow::animate() {
    try {
        if (!p_net)
            throw runtime_error("Сначала нужно произвести расчёты");

        auto speed = ui->sSpeedSB->value();
        auto logs = p_net->get_logs_per_transact();
        auto q_logs = p_net->get_q_logs();
        auto anim = new AnimationWindow(src, logs, mtime, speed, q_logs, this);
        anim->show();
    } catch (const exception &e) {
        auto msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Critical);
        msg->setText("Ошибка");
        msg->setInformativeText(e.what());
        msg->setWindowTitle("Ошибка");
        msg->exec();
    }
}
