#include "ReceptionAnimItem.h"

ReceptionAnimItem::ReceptionAnimItem(QGraphicsScene *scene, int x, int y, int size) {
    queue = new QueueAnimItem(x, y, size);
    reception = new WorkerAnimItem(x + size * 2, y, size);

    scene->addItem(queue);
    scene->addItem(reception);
}

QPoint ReceptionAnimItem::get_in_place() {
    return queue->get_in_place();
}

QPoint ReceptionAnimItem::get_out_place() {
    return reception->get_out_place();
}
