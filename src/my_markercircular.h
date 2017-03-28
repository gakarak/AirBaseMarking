#ifndef MYITEMGROUPCIRCULARMARKER_H
#define MYITEMGROUPCIRCULARMARKER_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>

#include "my_markerposition.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <QGraphicsScene>
#include "my_markerposition.h"
#include "my_itemcorner.h"
#include "my_itemcirculararea.h"
#include "my_markerbasic.h"

class QGraphicsSceneMouseEvent;
class ItemCorner;


class MarkerCircular : public MarkerBasic
{
public:
    explicit MarkerCircular(QGraphicsItem *parent = 0);

    int type() const {
        return TypeMarkerCircular;
    }

    bool loadFromCSVString(const QString& str, const QString& sep="|");

    virtual QString exportToCSVString(const DatasetConfig& cfg,
                                      const QString& sep="|");

    void placeOnScene(QGraphicsScene *scene);

    QRectF boundingRect() const;

public:
    ItemCorner        *ptrCorner;
    ItemCircularArea  *ptrCircleArea;
};

#endif // MYITEMGROUPCIRCULARMARKER_H
