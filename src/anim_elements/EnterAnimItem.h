#ifndef VKR_ENTERANIMITEM_H
#define VKR_ENTERANIMITEM_H

#include <QGraphicsRectItem>

#include "BaseAnimItem.h"

class EnterAnimItem : public BaseAnimItem, public QGraphicsRectItem {
private:
    QPointF center;
    QPointF in_point;
    QPointF out_point;
public:
    EnterAnimItem(int x, int y, int size);

    virtual QPointF get_center();

    virtual QPointF get_in_place();

    virtual QPointF get_out_place();
};

#endif
