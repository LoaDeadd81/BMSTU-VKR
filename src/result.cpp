#include "result.h"

#include <utility>

ResultWindow::ResultWindow(SMOStats stats, QWidget *parent)
        : QMainWindow{parent}, ui(new Ui::Result), stats(std::move(stats)) {
    ui->setupUi(this);

    set_system_result();
    set_queue_result();
    set_worker_result();
}

void ResultWindow::set_system_result() {
    ui->systemTW->setRowCount(1);

    ui->systemTW->setItem(0, 0,
                          new QTableWidgetItem(QString::number(stats.system_stats.total_req)));
    ui->systemTW->setItem(0, 1,
                          new QTableWidgetItem(QString::number(stats.system_stats.complete_perc * 100, 'f', 0)));
    ui->systemTW->setItem(0, 2,
                          new QTableWidgetItem(QString::number(stats.system_stats.leave_perc * 100, 'f', 0)));
}

void ResultWindow::set_queue_result() {
    ui->queueTW->setRowCount(stats.window_groups_queue_stats.size() + 1);

    set_queue_row(0, "Ресепшен", stats.reception_queue_stats);
    for (int i = 0; i < stats.window_groups_queue_stats.size(); ++i)
        set_queue_row(i + 1, format("ГО {}", i + 1), stats.window_groups_queue_stats[i]);
}

void ResultWindow::set_worker_result() {
    ui->windowsTW->setRowCount(stats.window_groups_stats.size() + 1);

    set_worker_row(0, "Ресепшен", stats.reception_stats);
    for (int i = 0; i < stats.window_groups_stats.size(); ++i)
        set_worker_row(i + 1, format("ГО {}", i + 1), stats.window_groups_stats[i]);
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
    ui->windowsTW->setItem(row, 0,
                           new QTableWidgetItem(QString::fromStdString(name)));
    ui->windowsTW->setItem(row, 1,
                           new QTableWidgetItem(QString::number(stat.complete)));
    ui->windowsTW->setItem(row, 2,
                           new QTableWidgetItem(QString::number(stat.util, 'f', 2)));
    ui->windowsTW->setItem(row, 3,
                           new QTableWidgetItem(QString::number(stat.avg_work_time, 'f', 2)));
}

