#ifndef VKR_RECEPTIONANIMITEM_H
#define VKR_RECEPTIONANIMITEM_H

#include <QGraphicsScene>

#include "QueueAnimItem.h"
#include "WorkerAnimItem.h"
#include "ArrowAnimItem.h"

class ReceptionAnimItem {
public:
    QueueAnimItem *queue;
    WorkerAnimItem *reception;

    ReceptionAnimItem() {};

    ReceptionAnimItem(QGraphicsScene *scene, int x, int y, int size);
};

#endif
