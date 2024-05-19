#ifndef VKR_BASEANIMITEM_H
#define VKR_BASEANIMITEM_H

#include <QPoint>

class BaseAnimItem {
    virtual QPointF get_center() = 0;

    virtual QPointF get_in_place() = 0;

    virtual QPointF get_out_place() = 0;
};

#endif
