#include "QueueAnimItem.h"

QueueAnimItem::QueueAnimItem(int x, int y, int size)
        : QGraphicsEllipseItem(x, y, size, size) {

    center = QPointF(x + size / 2, y + size / 2);
    in_point = QPointF(x, y + size / 2);
    out_point = QPointF(x + size, y + size / 2);
    leave_place = QPointF(x + size / 2, y + size);
    exit_place = QPointF(x + size / 2, y + size * 2);
}

QPointF QueueAnimItem::get_center() {
    return center;
}

QPointF QueueAnimItem::get_in_place() {
    return in_point;
}

QPointF QueueAnimItem::get_out_place() {
    return out_point;
}

QPointF QueueAnimItem::get_leave_place() {
    return leave_place;
}

QPointF QueueAnimItem::get_exit_place() {
    return exit_place;
}

