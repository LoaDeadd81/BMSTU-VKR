#ifndef VKR_TRANSACTANIMITEM_H
#define VKR_TRANSACTANIMITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QBrush>

#include "Colors.h"

class TransactAnimItem : public QObject, public QGraphicsEllipseItem {
Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
public:
    TransactAnimItem(int type, int x, int y, int size);

public slots:

    void on_animFinish();
};

#endif
