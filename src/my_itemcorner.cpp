#include "my_itemcorner.h"

#include <QDebug>

ItemCorner::ItemCorner(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    this->isAutoSize = false;
    this->radus = 2.0;
    this->penDefault    = QPen(Qt::red);
    this->penSelected   = QPen(Qt::green);
    this->penDefault.setWidthF(0.5);
    this->penSelected.setWidthF(0.5);
    //
    setFlag(QGraphicsItem::ItemIsSelectable,    true);
    setFlag(QGraphicsItem::ItemIsMovable,       true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void ItemCorner::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(isSelected()) {
        painter->setPen(this->penSelected);
    } else {
        painter->setPen(this->penDefault);
    }
    painter->drawPoint(0,0);
    painter->drawEllipse(QRectF(-this->radus,-this->radus,2.*this->radus,2.*this->radus));
}

void ItemCorner::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalCornerPos(event->scenePos());
    QGraphicsItem::mouseMoveEvent(event);
//    qDebug() << parentItem() << " : " << event->scenePos();
    refreshSize(event->scenePos());
}

QRectF ItemCorner::boundingRect() const
{
    return QRectF(-this->radus-penDefault.widthF()/2.,
                  -this->radus-penDefault.widthF()/2.,
                  2*this->radus+penDefault.widthF(),
                  2*this->radus+penDefault.widthF());
}

void ItemCorner::refreshSize(const QPointF& scenePos)
{
    if(getIsAutoSize()) {
        if(parentItem()!=NULL) {
            QVector2D dv = QVector2D(scenePos - parentItem()->scenePos());
            this->radus = 0.08*dv.length();
            if(this->radus<2.0) {
                this->radus = 2.0;
            }
        }
    }
}

bool ItemCorner::getIsAutoSize() const
{
    return isAutoSize;
}

void ItemCorner::setIsAutoSize(bool value)
{
    isAutoSize = value;
}

//void ItemCorner::setRoot(MarkerItemCircular *ptr)
//{
//    this->ptrMarker = ptr;
//}

