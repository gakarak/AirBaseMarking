#include "datasetutils.h"

#include <exception>
#include <stdexcept>

#include <QDebug>
#include <QString>
#include <QPointF>
#include <QFileInfo>
#include <QSettings>

#include <QTime>
#include <QDate>

#include <QtGlobal>
#include <limits>

//////////////////////////////////////////////////////////
ImageMeta::ImageMeta(const QString &path)
{
    this->path = path;
    this->isLoaded = false;
}

void ImageMeta::loadInfo()
{
    QFileInfo finfo(this->path);
    if(finfo.exists()) {
        this->path = finfo.absoluteFilePath();
        QSettings sett(path, QSettings::IniFormat);
        sett.beginGroup("meta");
        double parOriginX = sett.value("originX").toDouble();
        double parOriginY = sett.value("originY").toDouble();
        this->originXY = QPointF(parOriginX,-parOriginY);
        double parOriginLon = sett.value("originLon").toDouble();
        double parOriginLat = sett.value("originLat").toDouble();
        this->originBL = QPointF(parOriginLon, parOriginLat);
        double parCornUL_X = sett.value("cornUL_X").toDouble();
        double parCornUL_Y =-sett.value("cornUL_Y").toDouble();
        double parCornLR_X = sett.value("cornLR_X").toDouble();
        double parCornLR_Y =-sett.value("cornLR_Y").toDouble();
        this->cornUL_XY = QPointF(parCornUL_X, parCornUL_Y);
        this->cornBR_XY = QPointF(parCornLR_X, parCornLR_Y);
        this->centerXY  = 0.5*(this->cornUL_XY + this->cornBR_XY);
        this->pathImgMask = sett.value("pathRelative").toString();
        sett.endGroup();
        this->isLoaded = true;
    } else {
        throw std::runtime_error("cant find path : " + path.toStdString());
    }
}

void ImageMeta::updateImagePath(const QString &map, const QString &zoom)
{
    if(this->isLoaded) {
        this->pathImg = QString().sprintf(this->pathImgMask.toStdString().c_str(),
                                          zoom.toStdString().c_str(),
                                          map.toStdString().c_str(),
                                          zoom.toStdString().c_str());
    } else {
        throw std::runtime_error("ImageMeta is not correctly initialyzed");
    }
}

void ImageMeta::calcRelativeCoords(const QPointF &pcXY)
{
    this->cornUL_XYrel = this->cornUL_XY - pcXY;
    this->cornBR_XYrel = this->cornBR_XY - pcXY;
    this->centerXYrel  = this->centerXY  - pcXY;
}

double ImageMeta::getWidth() const
{
    double ret = qAbs(this->cornBR_XYrel.x() - this->cornUL_XYrel.x());
    return ret;
}

double ImageMeta::getHeight() const
{
    double ret = qAbs(this->cornBR_XYrel.y() - this->cornUL_XYrel.y());
    return ret;
}

//////////////////////////////////////////////////////////
DatasetConfig::DatasetConfig(const QString &path)
{
    this->pathCfg   = path;
    this->isLoaded  = false;
}

void DatasetConfig::loadConfig()
{
    QFileInfo finfo(this->pathCfg);
    if(finfo.exists()) {
        this->pathCfg = finfo.absoluteFilePath();
        this->wdir = finfo.absoluteDir();
        QSettings sett(this->pathCfg, QSettings::IniFormat);
        sett.beginGroup("config");
        this->currentZoom   = sett.value("loadScale").toString();
        this->listZoom      = sett.value("scales").toStringList();
        this->mapName       = sett.value("prefix").toString();
        this->listObjTypes = sett.value("objTypes").toStringList();
        this->mapObjSubtypes.clear();
        foreach (QString k, this->listObjTypes) {
            QString tk = QString("types/%1").arg(k);
            QStringList tlst = sett.value(tk).toStringList();
            this->mapObjSubtypes.insert(k, tlst);
        }
        QString tfn = sett.value("csvmarkers","").toString();
        if(!tfn.isEmpty()) {
            this->pathCSV = this->wdir.absoluteFilePath(tfn);
        } else {
            this->pathCSV = "";
        }
        sett.endGroup();
        sett.beginGroup("lstmeta");
        this->listMetaInfo.clear();
        QStringList lstMetaKey = sett.allKeys();
        foreach (QString k, lstMetaKey) {
            QString fnmeta = sett.value(k).toString();
            QString tpathMeta = this->wdir.absoluteFilePath(fnmeta);
            ImageMeta tmeta(tpathMeta);
            tmeta.loadInfo();
            this->listMetaInfo.append(tmeta);
        }
        sett.endGroup();
        this->isLoaded = true;
        this->prepareSceneCenter();
        this->saveRelativeImageCoods2Ini();
        this->setZoom();
    } else {
        throw std::runtime_error("cant find config file : " + this->pathCfg.toStdString());
    }
}

void DatasetConfig::checkIsLoaded() const
{
    if(!this->isLoaded) {
        throw std::runtime_error("DatasetConfig is not configured correctly...");
    }
}

void DatasetConfig::setZoom()
{
    this->checkIsLoaded();
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        this->listMetaInfo[ii].updateImagePath(this->mapName,this->currentZoom);
    }
}

void DatasetConfig::prepareSceneCenter()
{
    this->checkIsLoaded();
    this->sceneCenterXY = this->listMetaInfo.at(0).originXY;
    this->sceneCenterBL = this->listMetaInfo.at(0).originBL;
    // sort by X:
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        const ImageMeta& tmeta = this->listMetaInfo.at(ii);
        if(this->sceneCenterXY.x()>tmeta.originXY.x()) {
            this->sceneCenterXY.setX(tmeta.originXY.x());
            this->sceneCenterBL.setX(tmeta.originBL.x());
        }
    }
    // sort by Y:
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        const ImageMeta& tmeta = this->listMetaInfo.at(ii);
        if(this->sceneCenterXY.y()>tmeta.originXY.y()) {
            this->sceneCenterXY.setY(tmeta.originXY.y());
            this->sceneCenterBL.setY(tmeta.originBL.y());
        }
    }
    //
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        listMetaInfo[ii].calcRelativeCoords(this->sceneCenterXY);
    }
}

void DatasetConfig::saveRelativeImageCoods2Ini() const
{
    this->checkIsLoaded();
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        const ImageMeta& tmeta = listMetaInfo.at(ii);
        QSettings sett(tmeta.path, QSettings::IniFormat);
        sett.beginGroup("localxy");
        sett.setValue("posUL_X", QString("%1").arg(tmeta.cornUL_XYrel.x(),0,'g',12));
        sett.setValue("posUL_Y", QString("%1").arg(tmeta.cornUL_XYrel.y(),0,'g',12));
        sett.setValue("posBR_X", QString("%1").arg(tmeta.cornBR_XYrel.x(),0,'g',12));
        sett.setValue("posBR_Y", QString("%1").arg(tmeta.cornBR_XYrel.y(),0,'g',12));
    }
}

QRectF DatasetConfig::getBoundingBoxXYrel(double pad) const
{
    QRectF ret;
    this->checkIsLoaded();
    QPointF ptLU(+std::numeric_limits<double>::max(),+std::numeric_limits<double>::max());
    QPointF ptBR(-std::numeric_limits<double>::max(),-std::numeric_limits<double>::max());
    for(int ii=0; ii<this->listMetaInfo.size(); ii++) {
        const ImageMeta& tmeta = listMetaInfo.at(ii);
        // Left-Top
        if(ptLU.x()>tmeta.cornUL_XYrel.x()) {
            ptLU.setX(tmeta.cornUL_XYrel.x());
        }
        if(ptLU.y()>tmeta.cornUL_XYrel.y()) {
            ptLU.setY(tmeta.cornUL_XYrel.y());
        }
        // Bottom-Right
        if(ptBR.x()<tmeta.cornBR_XYrel.x()) {
            ptBR.setX(tmeta.cornBR_XYrel.x());
        }
        if(ptBR.y()<tmeta.cornBR_XYrel.y()) {
            ptBR.setY(tmeta.cornBR_XYrel.y());
        }
    }
    double tscale = 1.+2.*pad;
    double width  = qAbs(ptBR.x() - ptLU.x());
    double height = qAbs(ptBR.y() - ptLU.y());
    double newWidth  = tscale*width;
    double newHeight = tscale*height;
    double dx = width*pad;
    double dy = height*pad;
    ret.setX(ptLU.x()-dx);
    ret.setY(ptLU.y()-dy);
    ret.setWidth(newWidth);
    ret.setHeight(newHeight);
    return ret;
}

void DatasetConfig::saveNewCSVFileName(const QString &fnCSV) {
    if(isLoaded && (!fnCSV.isEmpty())) {
        QSettings sett(this->pathCfg, QSettings::IniFormat);
        sett.beginGroup("config");
        sett.setValue("csvmarkers",fnCSV);
        sett.endGroup();
        this->pathCSV = this->wdir.absoluteFilePath(fnCSV);
    }
}

void DatasetConfig::generateCSVFilenameAdd2Cfg(const QString &prefix) {
    if(isLoaded) {
        QString tfn = DatasetConfig::generateCSVFilename(prefix);
        this->saveNewCSVFileName(tfn);
    }
}

QString DatasetConfig::generateCSVFilename(const QString &prefix) {

    QDate dat   = QDate::currentDate();
    QTime tim   = QTime::currentTime();
    QString strIdx = QString().sprintf("%s-%d%02d%02d-%02d%02d%03d-%04d.csv",
                                       prefix.toStdString().c_str(),
                                       dat.year(), dat.month(), dat.day(),
                                       tim.hour(), tim.minute(), tim.second(),
                                       tim.msec());
    return strIdx;
}
