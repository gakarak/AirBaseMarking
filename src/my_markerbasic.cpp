#include "my_markerbasic.h"


QString MapMarkers::TAirplane   = "Airplane";
QString MapMarkers::TDontCare   = "DontCare";

//////////////////////////////////////////////////////
MarkerBasic::MarkerBasic(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    this->numParamStr = 11;
}

QString MarkerBasic::getMarkerType() const
{
    return markerType;
}

void MarkerBasic::setMarkerType(const QString &value)
{
    markerType = value;
}

QString MarkerBasic::getMarkerSubType() const
{
    return markerSubType;
}

void MarkerBasic::setMarkerSubType(const QString &value)
{
    markerSubType = value;
}

bool MarkerBasic::loadFromCSVString(const QString &str, const QString &sep) {
    qDebug() << "MarkerBasic::loadFromCSVStringBasic()";
    QStringList lstStr = str.split(sep);
    if(lstStr.size()>=numParamStr) {
        try {
            markerType      = lstStr.at(0);
            markerSubType   = lstStr.at(1);
            //
            QString s1 = "10.4";
            s1.toDouble();
            pts1 = QPointF(lstStr.at(2).toDouble(), lstStr.at(3).toDouble());
            pts2 = QPointF(lstStr.at(4).toDouble(), lstStr.at(5).toDouble());
            return true;
        } catch (std::exception& e) {
            qDebug() << "Error: " << e.what();
            return false;
        }
    } else {
        return false;
    }
}

QString MarkerBasic::exportToCSVString(const DatasetConfig &cfg, const QString &sep) {
    if(!cfg.isLoaded) {
        qDebug() << "I think, that you have not loaded dataset, skip export of ("
                 << markerType << "," << markerSubType << ")";
        return QString("");
    } else {
        qDebug() << "MarkerBasic::exportToCSVString()";
        QStringList tlst;
        tlst << markerType << markerSubType;
        // p1-XY
        tlst << QString("%1").arg(pts1.x(), 0, 'g', 16);
        tlst << QString("%1").arg(pts1.y(), 0, 'g', 16);
        // p2-XY
        tlst << QString("%1").arg(pts2.x(), 0, 'g', 16);
        tlst << QString("%1").arg(pts2.y(), 0, 'g', 16);
        // Global coords
        QPointF coordXYglob1,coordXYglob2,
                coordBL1, coordBL2;
        GeoUtils::calcGlobalCoordsFromSceneProj4(cfg, pts1,
                                                 coordXYglob1, coordBL1);
        GeoUtils::calcGlobalCoordsFromSceneProj4(cfg, pts2,
                                                 coordXYglob2, coordBL2);
        // p1-BL
        tlst << QString("%1").arg(coordBL1.x(), 0, 'g', 16);
        tlst << QString("%1").arg(coordBL1.y(), 0, 'g', 16);
        // p2-BL
        tlst << QString("%1").arg(coordBL2.x(), 0, 'g', 16);
        tlst << QString("%1").arg(coordBL2.y(), 0, 'g', 16);
        // size
        QVector2D siz = QVector2D(pts2 - pts1);
        tlst << QString("%1").arg(siz.length(), 0, 'g', 16);
        return tlst.join(sep);
    }
}

int MarkerBasic::getTypeMarkerFromCSVString(const QString &csvStr, const QString &sep)
{
    int len = csvStr.size();
    if( (len<1) || csvStr.startsWith("#")) {
        return -1;
    }
    int pos = csvStr.indexOf(sep);
    QString strType = csvStr.left(pos);
    if(strType==MapMarkers::TAirplane) {
        return TypeMarkerCircular;
    } else if(strType==MapMarkers::TDontCare) {
        return TypeMarkerRect;
    } else {
        return -1;
    }
}
