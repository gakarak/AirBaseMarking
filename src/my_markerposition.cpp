#include "my_markerposition.h"

#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

MarkerPosition::MarkerPosition(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    this->sizeRadius    = 10;
    this->penDefault    = QPen(Qt::red);
    this->penOnHover    = QPen(Qt::green);
    //
    setFlag(QGraphicsItem::ItemIsSelectable,    true);
    setFlag(QGraphicsItem::ItemIsMovable,       true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QRectF MarkerPosition::boundingRect() const
{
    return QRectF(-this->sizeRadius-2,-this->sizeRadius-2, 2*this->sizeRadius+4, 2*this->sizeRadius+4);
}

void MarkerPosition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(this->isSelected()) {
        painter->setPen(this->penDefault);
    } else {
        painter->setPen(this->penOnHover);
    }
    painter->drawEllipse(QRectF(-1.,-1.,2.,2.));
    painter->drawEllipse(QRectF(-this->sizeRadius,-this->sizeRadius, 2*this->sizeRadius, 2*this->sizeRadius));
}

void MarkerPosition::setSizeRadius(int rad)
{
    this->sizeRadius = rad;
}
