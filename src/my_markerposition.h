#ifndef MYGRAPHICSITEMMERKER_H
#define MYGRAPHICSITEMMERKER_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPen>

class QPen;
class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;
class QGraphicsSceneMouseEvent;

enum {
    TypeMarker              = QGraphicsItem::UserType +  1,
    TypeMarkerPosition      = QGraphicsItem::UserType +  2,
    TypeMarkerCircular      = QGraphicsItem::UserType +  3,
    TypeMarkerRect          = QGraphicsItem::UserType +  4,

    TypeItemCorner          = QGraphicsItem::UserType + 10,
    TypeItemCircularArea    = QGraphicsItem::UserType + 11,
    TypeItemRectArea        = QGraphicsItem::UserType + 12,
};

class MarkerPosition : public QGraphicsItem
{
public:
    explicit MarkerPosition(QGraphicsItem* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    void setSizeRadius(int rad);
    int type() const {
        return TypeMarkerPosition;
    }

    int     sizeRadius;
    QPen    penDefault;
    QPen    penOnHover;
};

#endif // MYGRAPHICSITEMMERKER_H
