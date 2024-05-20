#include "QSizeAnimItem.h"

QSizeAnimItem::QSizeAnimItem(int x, int y, int size) : QGraphicsTextItem(QString("0")) {
    this->setPos(x - size / 4, y - size / 2);

    QFont f;
    f.setPixelSize(size / 2);
    this->setFont(f);
}

void QSizeAnimItem::setText(int val) {
    this->setPlainText(QString::number(val));
}

int QSizeAnimItem::text() {
    return this->toPlainText().toInt();
}
