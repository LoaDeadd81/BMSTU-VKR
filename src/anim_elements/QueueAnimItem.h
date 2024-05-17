#ifndef VKR_QUEUEANIMITEM_H
#define VKR_QUEUEANIMITEM_H

#include <QGraphicsEllipseItem>

#include "BaseAnimItem.h"

class QueueAnimItem : public BaseAnimItem, public QGraphicsEllipseItem {
private:
    QPoint center;
    QPoint in_point;
    QPoint out_point;
public:
    QueueAnimItem(int x, int y, int size);

    virtual QPoint get_center();

    virtual QPoint get_in_place();

    virtual QPoint get_out_place();
};

#endif
