#ifndef GEOUTILS_H
#define GEOUTILS_H

#include "datasetutils.h"

class GeoUtils
{
private:
    GeoUtils();
public:
    static double getMPPForLatituze(double lat, int zoom);
    static void calcGlobalCoordsFromSceneGeoLib(const DatasetConfig& config,
                                                const QPointF& posScene,
                                                QPointF& coordXY,
                                                QPointF& coordBL);
    static void calcGlobalCoordsFromSceneProj4(const DatasetConfig& config,
                                               const QPointF& posScene,
                                               QPointF& coordXY,
                                               QPointF& coordBL);
};

#endif // GEOUTILS_H
