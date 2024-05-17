#ifndef VKR_BASEANIMITEM_H
#define VKR_BASEANIMITEM_H

#include <QPoint>

class BaseAnimItem {
    virtual QPoint get_center() = 0;

    virtual QPoint get_in_place() = 0;

    virtual QPoint get_out_place() = 0;
};

#endif
