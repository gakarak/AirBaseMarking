#include "hlgutils.h"

#include <QDebug>
#include <QString>
#include <QtMath>
#include <QPointF>
#include <QFileInfo>
#include <QSettings>

#include <GeographicLib/TransverseMercator.hpp>
#include <exception>



////////////////////////////////////////////
HLGUtils::HLGUtils() {}

HLGData HLGUtils::loadHLGData(const QString &path)
{
    HLGData hlg(path);
    hlg.loadHLG();
    return hlg;
}

HLGData HLGUtils::project2XY(const HLGData &hlg, bool isCentered)
{
    HLGData ret(hlg.path);
    ret.zoom = hlg.zoom;
    GeographicLib::TransverseMercator proj(
                GeographicLib::Constants::WGS84_a(),
                GeographicLib::Constants::WGS84_f(),
                GeographicLib::Constants::UTM_k0());
    QPointF pcnt    = hlg.getCenterBL();
    double lon0     = pcnt.x();
    double xtmp, ytmp, xc=0., yc=0.;
    // pc
    if(isCentered) {
        proj.Forward(lon0, pcnt.y(), pcnt.x(), xc, yc);
    }
    // pt0
    proj.Forward(lon0, hlg.pts0.y(), hlg.pts0.x(), xtmp, ytmp);
    ret.pts0 = QPointF(xtmp-xc, ytmp-yc);
    // pt1
    proj.Forward(lon0, hlg.pts1.y(), hlg.pts1.x(), xtmp, ytmp);
    ret.pts1 = QPointF(xtmp-xc, ytmp-yc);
    // pt2
    proj.Forward(lon0, hlg.pts2.y(), hlg.pts2.x(), xtmp, ytmp);
    ret.pts2 = QPointF(xtmp-xc, ytmp-yc);
    // pt3
    proj.Forward(lon0, hlg.pts3.y(), hlg.pts3.x(), xtmp, ytmp);
    ret.pts3 = QPointF(xtmp-xc, ytmp-yc);
    return ret;
}

////////////////////////////////////////////
void HLGData::loadHLG()
{
    QFileInfo finfo(this->path);
    if(finfo.exists()) {
        QSettings tmpSettings(this->path, QSettings::IniFormat);
        try {
            tmpSettings.beginGroup("HIGHLIGHTING");
            this->zoom  = tmpSettings.value("zoom").toInt();
            //
            // (p1)---(p2)
            //  |      |
            //  |      |
            // (p0)---(p3)
            //
            // lon -> x
            // lat -> y
            //
            // pt0
            double lon0 = tmpSettings.value("PointLon_0").toDouble();
            double lat0 = tmpSettings.value("PointLat_0").toDouble();
            this->pts0  = QPointF(lon0, lat0);
            // pt1
            double lon1 = tmpSettings.value("PointLon_1").toDouble();
            double lat1 = tmpSettings.value("PointLat_1").toDouble();
            this->pts1  = QPointF(lon1, lat1);
            // pt2
            double lon2 = tmpSettings.value("PointLon_2").toDouble();
            double lat2 = tmpSettings.value("PointLat_2").toDouble();
            this->pts2  = QPointF(lon2, lat2);
            // pt3
            double lon3 = tmpSettings.value("PointLon_3").toDouble();
            double lat3 = tmpSettings.value("PointLat_3").toDouble();
            this->pts3  = QPointF(lon3, lat3);
            tmpSettings.endGroup();
        } catch (std::exception &ex) {
            qDebug() << ex.what();
        }
    } else {
        throw std::runtime_error("cant fidn file: " + this->path.toStdString());
    }
}

QPointF HLGData::getCenterBL() const
{
    double meanLon = 0.5*(this->pts0.x() + this->pts2.x());
    double meanLat = 0.5*(this->pts0.y() + this->pts2.y());
    QPointF tret(meanLon, meanLat);
    return tret;
}

QString HLGData::toString(bool isInKm/*=false*/) const
{
    QPointF pc = this->getCenterBL();
    double k = 1.0;
    if(isInKm) {
        k = 1./1000.;
    }
    QString ret = QString().sprintf("Zoom(%d), p0(%f,%f), p1(%f,%f), p2(%f,%f), p3(%f,%f) -> pc(%f,%f)",
                                    this->zoom,
                                    this->pts0.x()*k,this->pts0.y()*k,
                                    this->pts1.x()*k,this->pts1.y()*k,
                                    this->pts2.x()*k,this->pts2.y()*k,
                                    this->pts3.x()*k,this->pts3.y()*k,
                                    pc.x()*k,pc.y()*k);
    return ret;
}
