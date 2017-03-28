#ifndef ITEMCORNER_H
#define ITEMCORNER_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include "my_markerposition.h"
#include "my_markercircular.h"

class ItemCorner : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ItemCorner(QGraphicsItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect() const;
    void refreshSize(const QPointF &scenePos);

    int type() const {
        return TypeItemCorner;
    }
signals:
    void signalCornerPos(const QPointF& post);
public:
    qreal radus;
    QPen penDefault;
    QPen penSelected;
    bool getIsAutoSize() const;
    void setIsAutoSize(bool value);

private:
    bool isAutoSize;
};

#endif // ITEMCORNER_H
