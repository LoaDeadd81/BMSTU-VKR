#include "QueueAnimItem.h"

QueueAnimItem::QueueAnimItem(int x, int y, int size)
        : QGraphicsEllipseItem(x, y, size, size) {

    center = QPoint(x + size / 2, y + size / 2);
    in_point = QPoint(x, y + size / 2);
    out_point = QPoint(x + size, y + size / 2);
    leave_place = QPoint(x + size / 2, y + size);
    exit_place = QPoint(x + size / 2, y + size * 2);
}

QPoint QueueAnimItem::get_center() {
    return center;
}

QPoint QueueAnimItem::get_in_place() {
    return in_point;
}

QPoint QueueAnimItem::get_out_place() {
    return out_point;
}

QPoint QueueAnimItem::get_leave_place() {
    return leave_place;
}

QPoint QueueAnimItem::get_exit_place() {
    return exit_place;
}

