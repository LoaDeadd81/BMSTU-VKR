#ifndef VKR_ENTERANIMITEM_H
#define VKR_ENTERANIMITEM_H

#include <QGraphicsRectItem>

#include "BaseAnimItem.h"

class EnterAnimItem : public BaseAnimItem, public QGraphicsRectItem {
private:
    QPoint center;
    QPoint in_point;
    QPoint out_point;
public:
    EnterAnimItem(int x, int y, int size);

    virtual QPoint get_center();

    virtual QPoint get_in_place();

    virtual QPoint get_out_place();
};

#endif
