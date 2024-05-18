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

    virtual QPoint get_center();

    virtual QPoint get_in_place();

    virtual QPoint get_out_place();

    virtual QPoint get_exit_place();
};

#endif
