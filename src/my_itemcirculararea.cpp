#include "my_itemcirculararea.h"

#include <QColor>
#include <QVector2D>
#include <QDebug>

ItemCircularArea::ItemCircularArea(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->coef   = 0.8;
    this->radius = 12.0;
    this->penDefault    = QPen(Qt::red);
    this->penSelected   = QPen(Qt::green);
    this->penDefault.setWidthF(1.0);
    this->penSelected.setWidthF(1.0);
    this->brush = QBrush(QColor(200,0,0,96));
}

void ItemCircularArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(isSelected()) {
        painter->setPen(this->penSelected);
    } else {
        painter->setPen(this->penDefault);
    }
    painter->setBrush(this->brush);
    painter->drawEllipse(QPointF(0.,0.),
                         1.*this->radius,
                         1.*this->radius);
    painter->drawPoint(0,0);
    painter->drawLine(QLineF(QPointF(0.,0.),this->radiusVector.toPointF()));
}

QRectF ItemCircularArea::boundingRect() const
{
    return QRectF(-this->radius - 2.*this->penDefault.widthF()/2.,
                  -this->radius - 2.*this->penDefault.widthF()/2.,
                  2.*this->radius + 4.*this->penDefault.widthF()/2.,
                  2.*this->radius + 4.*this->penDefault.widthF()/2.);
}

void ItemCircularArea::slotUpdateRadius(const QPointF &pos)
{
    this->radiusVector  = this->coef*(QVector2D(pos) - QVector2D(this->scenePos()));
    this->radius = this->radiusVector.length();
    qDebug() << "ItemCircularArea::" << this->scenePos() << " --> " << pos << " : "
             << this->radius << " / " << this->radiusVector;
    this->update();
}
