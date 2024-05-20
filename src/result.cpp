#include "result.h"

#include <utility>

ResultWindow::ResultWindow(SMOStats stats, QWidget *parent)
        : QMainWindow{parent}, ui(new Ui::Result), stats(std::move(stats)) {
    ui->setupUi(this);

    set_system_result();
    set_queue_result();
    set_worker_result();
    set_groups_result();
    set_types_result();
}

void ResultWindow::set_system_result() {
    ui->systemTW->setRowCount(1);

    ui->systemTW->setItem(0, 0,
                          new QTableWidgetItem(QString::number(stats.system_stats.total_req)));
    ui->systemTW->setItem(0, 1,
                          new QTableWidgetItem(QString::number(stats.system_stats.complete)));
    ui->systemTW->setItem(0, 2,
                          new QTableWidgetItem(QString::number(stats.system_stats.leave)));
}

void ResultWindow::set_queue_result() {
    ui->queueTW->setRowCount(stats.window_groups_queue_stats.size() + 1);

    set_queue_row(0, "Ресепшен", stats.reception_queue_stats);
    for (int i = 0; i < stats.window_groups_queue_stats.size(); ++i)
        set_queue_row(i + 1, format("ГО {}", i + 1), stats.window_groups_queue_stats[i]);
}

void ResultWindow::set_worker_result() {
    ui->workerTW->setRowCount(1);

    set_worker_row(0, "Ресепшен", stats.reception_stats);

}

void ResultWindow::set_groups_result() {
    ui->windowsTW->setRowCount(stats.window_groups_stats.size());

    for (int i = 0; i < stats.window_groups_stats.size(); ++i)
        set_group_row(i, format("ГО {}", i + 1), stats.window_groups_stats[i]);
}

void ResultWindow::set_types_result() {
    ui->typesTW->setRowCount(stats.type_stats.size());

    for (int i = 0; i < stats.type_stats.size(); ++i)
        set_type_row(i, format("ГЗ {}", i + 1), stats.type_stats[i]);
}


void ResultWindow::set_queue_row(int row, string name, QueueStats stat) {
    ui->queueTW->setItem(row, 0,
                         new QTableWidgetItem(QString::fromStdString(name)));
    ui->queueTW->setItem(row, 1,
                         new QTableWidgetItem(QString::number(stat.max)));
    ui->queueTW->setItem(row, 2,
                         new QTableWidgetItem(QString::number(stat.entries)));
    ui->queueTW->setItem(row, 3,
                         new QTableWidgetItem(QString::number(stat.empty_entries)));
    ui->queueTW->setItem(row, 4,
                         new QTableWidgetItem(QString::number(stat.avg_size, 'f', 2)));
    ui->queueTW->setItem(row, 5,
                         new QTableWidgetItem(QString::number(stat.avg_time, 'f', 2)));
    ui->queueTW->setItem(row, 6,
                         new QTableWidgetItem(QString::number(stat.leave_num)));
}

void ResultWindow::set_worker_row(int row, string name, WorkerStats stat) {
    ui->workerTW->setItem(row, 0,
                          new QTableWidgetItem(QString::fromStdString(name)));
    ui->workerTW->setItem(row, 1,
                          new QTableWidgetItem(QString::number(stat.complete)));
    ui->workerTW->setItem(row, 2,
                          new QTableWidgetItem(QString::number(stat.util, 'f', 2)));
    ui->workerTW->setItem(row, 3,
                          new QTableWidgetItem(QString::number(stat.avg_work_time, 'f', 2)));
}

void ResultWindow::set_group_row(int row, string name, GroupStats stat) {
    ui->windowsTW->setItem(row, 0,
                           new QTableWidgetItem(QString::fromStdString(name)));
    ui->windowsTW->setItem(row, 1,
                           new QTableWidgetItem(QString::number(stat.cap)));
    ui->windowsTW->setItem(row, 2,
                           new QTableWidgetItem(QString::number(stat.complete)));
    ui->windowsTW->setItem(row, 3,
                           new QTableWidgetItem(QString::number(stat.util, 'f', 2)));
    ui->windowsTW->setItem(row, 4,
                           new QTableWidgetItem(QString::number(stat.avg_cnt, 'f', 2)));
    ui->windowsTW->setItem(row, 5,
                           new QTableWidgetItem(QString::number(stat.avg_work_time, 'f', 2)));
}

void ResultWindow::set_type_row(int row, string name, TypeStats stat) {
    ui->typesTW->setItem(row, 0,
                         new QTableWidgetItem(QString::fromStdString(name)));
    ui->typesTW->setItem(row, 1,
                         new QTableWidgetItem(QString::number(stat.gen)));
    ui->typesTW->setItem(row, 2,
                         new QTableWidgetItem(QString::number(stat.complete)));
    ui->typesTW->setItem(row, 3,
                         new QTableWidgetItem(QString::number(stat.leave)));
    ui->typesTW->setItem(row, 4,
                         new QTableWidgetItem(QString::number(stat.q_avg_time, 'f', 2)));
    ui->typesTW->setItem(row, 5,
                         new QTableWidgetItem(QString::number(stat.w_avg_time, 'f', 2)));
}

