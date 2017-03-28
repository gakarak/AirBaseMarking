#include "my_itemrectarea.h"

#include <QDebug>

ItemRectArea::ItemRectArea(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    this->coefMarker    = 0.8;

    this->ptsUL = QPointF(-10.,-10.);
    this->ptsBR = QPointF(+10.,+10.);

    this->penDefault    = QPen(Qt::blue);
    this->penSelected   = QPen(Qt::cyan);
    this->penDefault.setWidthF(1.0);
    this->penSelected.setWidthF(1.0);
    this->brush = QBrush(QColor(0,0,200,96));
}

void ItemRectArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(isSelected()) {
        painter->setPen(this->penSelected);
    } else {
        painter->setPen(this->penDefault);
    }
    painter->setBrush(this->brush);
    painter->drawRect(QRectF(this->ptsUL, this->ptsBR));
    //
//    qDebug() << "p00: " << ptsUL << ", p11: " << ptsBR;
}

QRectF ItemRectArea::boundingRect() const {
    return QRectF(ptsUL*1.1,ptsBR*1.1);
}

void ItemRectArea::slotUpdateCornerUL(const QPointF &pos)
{
    QVector2D tmp   = QVector2D(pos) - QVector2D(this->scenePos());
    this->ptsUL     = this->coefMarker*tmp.toPointF();
    this->update();
}

void ItemRectArea::slotUpdateCornerBR(const QPointF &pos)
{
    QVector2D tmp   = QVector2D(pos) - QVector2D(this->scenePos());
    this->ptsBR     = this->coefMarker*tmp.toPointF();
    this->ptsUL     = -this->ptsBR;
    this->update();
}
