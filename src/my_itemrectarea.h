#ifndef ITEMRECTAREA_H
#define ITEMRECTAREA_H

#include <QGraphicsItem>
#include <QObject>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QVector2D>
#include <QRectF>
#include <QPen>
#include <QBrush>

#include "my_markerposition.h"

class ItemRectArea : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ItemRectArea(QGraphicsItem* parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    int type() const {
        return TypeItemRectArea;
    }

    QRectF boundingRect() const;

public slots:
    void slotUpdateCornerUL(const QPointF& pos);
    void slotUpdateCornerBR(const QPointF& pos);

public:
    qreal       coefMarker; // (X,Y) of corner of rect is equal: coefMarker*(X_marker,Y_marker)
    QPointF     ptsUL, ptsBR;
    //
    QPen        penDefault;
    QPen        penSelected;
    QBrush      brush;
};

#endif // ITEMRECTAREA_H
