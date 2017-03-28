#ifndef ITEMCIRCULARAREA_H
#define ITEMCIRCULARAREA_H

#include <QGraphicsItem>
#include <QVector2D>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QObject>

#include "my_markerposition.h"

class ItemCircularArea : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ItemCircularArea(QGraphicsItem* parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    QRectF boundingRect() const;

    int type() const {
        return TypeItemCircularArea;
    }

public slots:
    void slotUpdateRadius(const QPointF& pos);

public:
    qreal       coef;
    QVector2D   radiusVector;
    qreal       radius;
    QPen        penDefault;
    QPen        penSelected;
    QBrush      brush;
};

#endif // ITEMCIRCULARAREA_H
