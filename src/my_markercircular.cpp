#include "my_markercircular.h"


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QDebug>

#include <QApplication>

MarkerCircular::MarkerCircular(QGraphicsItem *parent)
    :MarkerBasic(parent)
{

    ptrCircleArea   = new ItemCircularArea(this);
    ptrCorner   = new ItemCorner(this);
    addToGroup(ptrCorner);
    addToGroup(ptrCircleArea);

    ptrCorner->setPos(0,ptrCircleArea->radius);
    ptrCircleArea->slotUpdateRadius(ptrCorner->scenePos());

    ptrCorner->setFlag(QGraphicsItem::ItemIsMovable,       true);
    ptrCorner->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setFlag(QGraphicsItem::ItemIsSelectable,    true);
    setFlag(QGraphicsItem::ItemIsMovable,       true);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setFiltersChildEvents(false);
    setHandlesChildEvents(false);

    QApplication::connect(ptrCorner, SIGNAL(signalCornerPos(QPointF)),
                          ptrCircleArea, SLOT(slotUpdateRadius(QPointF)));
}

bool MarkerCircular::loadFromCSVString(const QString &str, const QString &sep) {
    qDebug() << "MarkerCircular::loadFromCSVString()";
    return MarkerBasic::loadFromCSVString(str, sep);
}

QString MarkerCircular::exportToCSVString(const DatasetConfig &cfg, const QString &sep) {
    this->pts1 = ptrCircleArea->scenePos();
    this->pts2 = ptrCircleArea->scenePos() + ptrCircleArea->radiusVector.toPointF();
    return MarkerBasic::exportToCSVString(cfg, sep);
}

void MarkerCircular::placeOnScene(QGraphicsScene *scene) {
    QVector2D vrel = QVector2D(pts2 - pts1);
    ptrCircleArea->radius       = vrel.length();
    ptrCircleArea->radiusVector = vrel;
    ptrCircleArea->setPos(QPointF(0., 0.));
    ptrCorner->setPos( (vrel/ptrCircleArea->coef).toPointF() );
    setPos(pts1);
    scene->addItem(this);
}

QRectF MarkerCircular::boundingRect() const
{
    return QRectF(-20,-20,40,40);
    /*
    return QRectF(-this->radius - this->penDefault.widthF()/2.,
                  -this->radius - this->penDefault.widthF()/2.,
                  2.*this->radius + 2.*this->penDefault.widthF()/2.,
                  2.*this->radius + 2.*this->penDefault.widthF()/2.);
    */
}
