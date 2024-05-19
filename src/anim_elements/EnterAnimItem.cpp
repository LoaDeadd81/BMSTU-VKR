#include "EnterAnimItem.h"

EnterAnimItem::EnterAnimItem(int x, int y, int size)
        : QGraphicsRectItem(x, y, size / 5, size) {

    center = QPointF(x + size / 10, y + size / 2);
    in_point = QPointF(x, y + size / 2);
    out_point = QPointF(x + size / 5, y + size / 2);
}

QPointF EnterAnimItem::get_center() {
    return center;
}

QPointF EnterAnimItem::get_in_place() {
    return in_point;
}

QPointF EnterAnimItem::get_out_place() {
    return out_point;
}
