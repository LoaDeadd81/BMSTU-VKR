#include "EnterAnimItem.h"

EnterAnimItem::EnterAnimItem(int x, int y, int size)
        : QGraphicsRectItem(x, y, size / 5, size) {

    center = QPoint(x + size / 10, y + size / 2);
    in_point = QPoint(x, y + size / 2);
    out_point = QPoint(x + size / 5, y + size / 2);
}

QPoint EnterAnimItem::get_center() {
    return center;
}

QPoint EnterAnimItem::get_in_place() {
    return in_point;
}

QPoint EnterAnimItem::get_out_place() {
    return out_point;
}
