#include "ArrowAnimItem.h"

void draw_arrow(QGraphicsScene *scene, QPointF start, QPointF end, int arrow_size) {
    qreal arrowSize = arrow_size / 4;

    QLineF line(end, start);
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    QBrush brush(Qt::black);
    scene->addPolygon(arrowHead, QPen(), brush);

    auto line_item = new QGraphicsLineItem(line);
    scene->addItem(line_item);
}
