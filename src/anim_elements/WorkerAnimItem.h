#ifndef VKR_WORKERANIMITEM_H
#define VKR_WORKERANIMITEM_H

#include <QGraphicsRectItem>

#include "BaseAnimItem.h"


class WorkerAnimItem : public BaseAnimItem, public QGraphicsRectItem {
private:
    QPoint center;
    QPoint in_point;
    QPoint out_point;
    QPoint exit_point;
public:
    WorkerAnimItem(int x, int y, int size);

    virtual QPointF get_center();

    virtual QPointF get_in_place();

    virtual QPointF get_out_place();

    virtual QPoint get_exit_place();
};

#endif
