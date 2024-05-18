#ifndef VKR_QUEUEANIMITEM_H
#define VKR_QUEUEANIMITEM_H

#include <QGraphicsEllipseItem>

#include "BaseAnimItem.h"

class QueueAnimItem : public BaseAnimItem, public QGraphicsEllipseItem {
private:
    QPoint center;
    QPoint in_point;
    QPoint out_point;
    QPoint leave_place;
    QPoint exit_place;
public:
    QueueAnimItem(int x, int y, int size);

    virtual QPoint get_center();

    virtual QPoint get_in_place();

    virtual QPoint get_out_place();

    QPoint get_leave_place();

    QPoint get_exit_place();
};

#endif
