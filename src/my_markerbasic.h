#ifndef MARKERBASIC_H
#define MARKERBASIC_H

#include <QGraphicsItemGroup>
#include <QPointF>
#include <QVector2D>
#include <QStringList>

#include <QDebug>

#include <exception>

#include <QGraphicsScene>
#include "my_markerposition.h"
#include "datasetutils.h"
#include "geoutils.h"

class MapMarkers {
private:
    MapMarkers(){}
public:
    static QString TAirplane;
    static QString TDontCare;
};

class MarkerBasic : public QGraphicsItemGroup
{
public:
    explicit MarkerBasic(QGraphicsItem* parent = 0);
    virtual ~MarkerBasic() {}

    QString getMarkerType() const;
    void setMarkerType(const QString &value);

    QString getMarkerSubType() const;
    void setMarkerSubType(const QString &value);

    /*
     * CSV Format:
     * XY - local, BL in Deg (only for debug)
     * only 2 parameters:
     *    1. for Circle: p1 - center, p2 - arrow
     *    2. for Rect: p1 - LeftUp corner, p2 - BottomRight corner
     * size in meters, only for debug
     * Marker-Type|Marker-Sub-Type|p1.x|p1.y|p2.x|p2.y|p1.Lon|p1.Lat|p2.Lon|p2.Lat|size
     * #parameters is 11
     */
    virtual bool loadFromCSVString(const QString& str, const QString& sep="|");
    virtual QString exportToCSVString(const DatasetConfig& cfg, const QString& sep="|");

    virtual void placeOnScene(QGraphicsScene* scene) = 0;

public :
    static int getTypeMarkerFromCSVString(const QString& csvStr,
                                          const QString& sep = "|");
    static QString getCSVHeader(const QString& sep = "|") {
        QStringList tlst;
        tlst << "#Type"
             << "SubType"
             << "p1x" << "p1y"
             << "p2x" << "p2y"
             << "p1Lon" << "p1Lat"
             << "p2Lon" << "p2Lat"
             << "size";
        return tlst.join(sep);
    }

protected:
    QString markerType;
    QString markerSubType;

    int numParamStr;
    QPointF pts1, pts2;
};

#endif // MARKERBASIC_H
