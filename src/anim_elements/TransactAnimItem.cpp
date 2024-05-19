#include "TransactAnimItem.h"

TransactAnimItem::TransactAnimItem(int type, int x, int y, int size)
        : QGraphicsEllipseItem(x - size / 4, y - size / 4, size / 2, size / 2) {
    this->setBrush(QBrush(color_vec[type]));
}

void TransactAnimItem::on_animFinish() {
    delete this;
}


