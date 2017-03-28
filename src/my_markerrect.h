#ifndef MARKERITEMRECT_H
#define MARKERITEMRECT_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>

#include "my_markerposition.h"
#include "my_itemcorner.h"
#include "my_itemrectarea.h"
#include "my_markerbasic.h"

class MarkerRect : public MarkerBasic
{
public:
    explicit MarkerRect(QGraphicsItem *parent = 0);

    int type() const {
        return TypeMarkerRect;
    }
    QRectF boundingRect() const;

    bool loadFromCSVString(const QString& str, const QString& sep="|");

    virtual QString exportToCSVString(const DatasetConfig& cfg,
                                      const QString& sep="|");

    void placeOnScene(QGraphicsScene* scene) {
        QPointF p0  = 0.5*(pts1 + pts2);
        QPointF pUL = pts1 - p0;
        QPointF pBR = pts2 - p0;
        ptrRectArea->setPos(0.,0.);
        ptrRectArea->ptsUL = pUL;
        ptrRectArea->ptsBR = pBR;
        ptrCornerBR->setPos(pBR/ptrRectArea->coefMarker);
        ptrCornerBR->refreshSize(ptrCornerBR->scenePos());
        setPos(p0);
        scene->addItem(this);
    }

public:
//    ItemCorner      *ptrCornerUL;
    ItemCorner      *ptrCornerBR;
    ItemRectArea    *ptrRectArea;
};

#endif // MARKERITEMRECT_H
