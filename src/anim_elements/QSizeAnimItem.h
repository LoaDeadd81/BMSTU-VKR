#ifndef VKR_QSIZEANIMITEM_H
#define VKR_QSIZEANIMITEM_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QString>
#include <QFont>

class QSizeAnimItem : public QGraphicsTextItem {
Q_OBJECT
    Q_PROPERTY(int txt READ text WRITE setText)
public:
    QSizeAnimItem(int x, int y, int size);

    void setText(int val);

    int text();
};

#endif
