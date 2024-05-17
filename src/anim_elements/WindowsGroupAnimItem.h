#ifndef VKR_WINDOWSGROUPANIMITEM_H
#define VKR_WINDOWSGROUPANIMITEM_H

#include <QGraphicsScene>
#include <vector>

#include "QueueAnimItem.h"
#include "WorkerAnimItem.h"

using namespace std;

class WindowsGroupAnimItem {
public:
    int size, padding;
    QueueAnimItem *queue;
    vector<WorkerAnimItem *> workers;

    WindowsGroupAnimItem(QGraphicsScene *scene, int x, int y, int size, int workers_num, int padding);

    int get_size();
};

#endif
