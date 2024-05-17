#ifndef VKR_RECEPTIONANIMITEM_H
#define VKR_RECEPTIONANIMITEM_H

#include <QGraphicsScene>

#include "QueueAnimItem.h"
#include "WorkerAnimItem.h"

class ReceptionAnimItem {
public:
    QueueAnimItem *queue;
    WorkerAnimItem *reception;

    ReceptionAnimItem(QGraphicsScene *scene, int x, int y, int size);

    QPoint get_in_place();

    QPoint get_out_place();
};

#endif
