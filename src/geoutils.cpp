#include "geoutils.h"

#include <QtMath>

#define TM_TX_MAXPOW 8
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/TransverseMercator.hpp>

#include <proj_api.h>

using namespace GeographicLib;


////////////////////////////////////////////////////////////////
GeoUtils::GeoUtils() {}

double GeoUtils::getMPPForLatituze(double lat, int zoom)
{
    double tret = 156543.03392 * qCos(lat * M_PI / 180.) / qPow(2, zoom - 1);
    return tret;
}

void GeoUtils::calcGlobalCoordsFromSceneGeoLib(const DatasetConfig &config,
                                               const QPointF &posScene,
                                               QPointF &coordXY,
                                               QPointF &coordBL)
{
    // a equatorial radius (meters)
    GeographicLib::Math::real equatorialRadius
            = GeographicLib::Constants::WGS84_a();
    // flattening of ellipsoid.  Setting f = 0 gives a sphere
    GeographicLib::Math::real flatteningOfEllipsoid = 0.0;
            //= GeographicLib::Constants::WGS84_f();
    // k0 central scale factor, in WGS84, equal 1.0??
    GeographicLib::Math::real centralScaleFactor = 1.0;
            //= GeographicLib::Constants::UTM_k0();
    //
    GeographicLib::TransverseMercator proj(
                equatorialRadius,
                flatteningOfEllipsoid,
                centralScaleFactor);
    double x0 = +posScene.x();
    double y0 = -posScene.y();//-this->config.sceneCenterXY.y();
    double dLon, dLat;
    double lon0 = config.sceneCenterBL.x();
    double gamma, k;
    proj.Reverse(lon0,x0,y0, dLat,dLon, gamma,k);
    double posLon = dLon;
    double posLat = dLat + config.sceneCenterBL.y();
    // xy:
    coordXY.setX(x0+config.sceneCenterXY.x());
    coordXY.setY(y0-config.sceneCenterXY.y());
    // Lon/Lat:
    coordBL.setX(posLon);
    coordBL.setY(posLat);
}

void GeoUtils::calcGlobalCoordsFromSceneProj4(const DatasetConfig &config,
                                              const QPointF &posScene,
                                              QPointF &coordXY,
                                              QPointF &coordBL)
{
    projPJ pj_merc, pj_latlong;
    double x0 = +posScene.x() + config.sceneCenterXY.x();
    double y0 = -posScene.y() - config.sceneCenterXY.y();
    double posX2Lon = x0;
    double posY2Lat = y0;
    pj_merc     = pj_init_plus("+proj=merc +init=epsg:3785");
    pj_latlong  = pj_init_plus("+proj=latlong");
    pj_transform(pj_merc, pj_latlong, 1, 1, &posX2Lon, &posY2Lat, NULL);
    posX2Lon *= RAD_TO_DEG;
    posY2Lat *= RAD_TO_DEG;
    // xy:
    coordXY.setX(x0);
    coordXY.setY(y0);
    // Lon/Lat:
    coordBL.setX(posX2Lon);
    coordBL.setY(posY2Lat);
}
