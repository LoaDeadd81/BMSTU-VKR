#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <format>

#include "Stats.h"

#include "ui_result.h"

class ResultWindow : public QMainWindow {
Q_OBJECT
public:
    explicit ResultWindow(SMOStats stats, QWidget *parent = nullptr);

private:
    Ui::Result *ui;

    SMOStats stats;

    void set_system_result();

    void set_queue_result();

    void set_worker_result();

    void set_groups_result();

    void set_types_result();

    void set_queue_row(int row, string name, QueueStats stat);

    void set_worker_row(int row, string name, WorkerStats stat);

    void set_group_row(int row, string name, GroupStats stat);

    void set_type_row(int row, string name, TypeStats stat);
};

#endif // RESULTWINDOW_H
