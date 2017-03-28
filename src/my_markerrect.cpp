#include "my_markerrect.h"

#include <QApplication>

MarkerRect::MarkerRect(QGraphicsItem *parent)
    : MarkerBasic(parent)
{
    ptrRectArea = new ItemRectArea(this);
//    ptrCornerUL = new ItemCorner(this);
    ptrCornerBR = new ItemCorner(this);
    ptrCornerBR->setIsAutoSize(true);

    addToGroup(ptrRectArea);
//    addToGroup(ptrCornerUL);
    addToGroup(ptrCornerBR);

//    ptrCornerUL->setPos(-16.,-16.);
    ptrCornerBR->setPos(+16.,+16.);
//    ptrRectArea->slotUpdateCornerUL(ptrCornerUL->scenePos());
    ptrRectArea->slotUpdateCornerBR(ptrCornerBR->scenePos());

//    ptrCornerUL->setFlag(QGraphicsItem::ItemIsMovable,              true);
//    ptrCornerUL->setFlag(QGraphicsItem::ItemSendsGeometryChanges,   true);

    ptrCornerBR->setFlag(QGraphicsItem::ItemIsMovable,              true);
    ptrCornerBR->setFlag(QGraphicsItem::ItemSendsGeometryChanges,   true);

    setFlag(QGraphicsItem::ItemIsSelectable,    true);
    setFlag(QGraphicsItem::ItemIsMovable,       true);

    setFiltersChildEvents(false);
    setHandlesChildEvents(false);

//    QApplication::connect(ptrCornerUL, SIGNAL(signalCornerPos(QPointF)),
//                          ptrRectArea, SLOT(slotUpdateCornerUL(QPointF)));
    QApplication::connect(ptrCornerBR, SIGNAL(signalCornerPos(QPointF)),
                          ptrRectArea, SLOT(slotUpdateCornerBR(QPointF)));
}

QRectF MarkerRect::boundingRect() const {
    return QRectF(-20.,-20.,40.,40.);
}

bool MarkerRect::loadFromCSVString(const QString &str, const QString &sep) {
    return MarkerBasic::loadFromCSVString(str, sep);
}

QString MarkerRect::exportToCSVString(const DatasetConfig &cfg, const QString &sep)
{
    this->pts1 = ptrRectArea->scenePos() + ptrRectArea->ptsUL;
    this->pts2 = ptrRectArea->scenePos() + ptrRectArea->ptsBR;
    return MarkerBasic::exportToCSVString(cfg, sep);
}
