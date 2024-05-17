#include "WindowsGroupAnimItem.h"

WindowsGroupAnimItem::WindowsGroupAnimItem(QGraphicsScene *scene, int x, int y, int size, int workers_num,
                                           int padding) : size(size), padding(padding) {
    int v_center = y + (size * workers_num + padding * (workers_num - 1)) / 2;

    queue = new QueueAnimItem(x, v_center - size / 2, size);
    scene->addItem(queue);

    workers = vector<WorkerAnimItem *>(workers_num);
    int worker_x = x + size * 3;
    for (int i = 0; i < workers_num; ++i) {
        workers[i] = new WorkerAnimItem(worker_x, y + i * (size + padding), size);
        scene->addItem(workers[i]);
    }
}

int WindowsGroupAnimItem::get_size() {
    return size * workers.size() + padding * (workers.size() - 1);
}
