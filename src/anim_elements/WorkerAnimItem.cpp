#include "WorkerAnimItem.h"

WorkerAnimItem::WorkerAnimItem(int x, int y, int size)
        : QGraphicsRectItem(x, y, size, size) {

    center = QPoint(x + size / 2, y + size / 2);
    in_point = QPoint(x, y + size / 2);
    out_point = QPoint(x + size, y + size / 2);
    exit_point = QPoint(x + size * 2, y + size / 2);
}

QPointF WorkerAnimItem::get_center() {
    return center;
}

QPointF WorkerAnimItem::get_in_place() {
    return in_point;
}

QPointF WorkerAnimItem::get_out_place() {
    return out_point;
}

QPoint WorkerAnimItem::get_exit_place() {
    return exit_point;
}
