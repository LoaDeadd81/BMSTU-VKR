#ifndef VKR_QUEUEANIMITEM_H
#define VKR_QUEUEANIMITEM_H

#include <QGraphicsEllipseItem>

#include "BaseAnimItem.h"

class QueueAnimItem : public BaseAnimItem, public QGraphicsEllipseItem {
private:
    QPointF center;
    QPointF in_point;
    QPointF out_point;
    QPointF leave_place;
    QPointF exit_place;
public:
    QueueAnimItem(int x, int y, int size);

    virtual QPointF get_center();

    virtual QPointF get_in_place();

    virtual QPointF get_out_place();

    QPointF get_leave_place();

    QPointF get_exit_place();
};

#endif
