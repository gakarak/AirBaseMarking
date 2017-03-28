#ifndef DATASETUTILS_H
#define DATASETUTILS_H

#include <QString>
#include <QPointF>
#include <QRectF>
#include <QVector>
#include <QStringList>
#include <QMap>
#include <QDir>

class ImageMeta {
public:
    ImageMeta(const QString& path = "");
    ~ImageMeta() {}
    void loadInfo();
    void updateImagePath(const QString& map, const QString& zoom);
    void calcRelativeCoords(const QPointF& pcXY);
    double getWidth() const;
    double getHeight() const;
    bool isLoaded;
    QString path;
    QPointF originXY, originBL;
    QPointF cornUL_XY, cornBR_XY, centerXY;
    QPointF cornUL_XYrel, cornBR_XYrel, centerXYrel;
    QString pathImgMask, pathImg;
};

class DatasetConfig {
public:
    DatasetConfig(const QString& path = "");
    ~DatasetConfig() {}
    void loadConfig();
    void checkIsLoaded() const;
    void setZoom();
    void prepareSceneCenter();
    void saveRelativeImageCoods2Ini() const;
    QRectF getBoundingBoxXYrel(double pad=0.) const;
    void saveNewCSVFileName(const QString& fnCSV);
    void generateCSVFilenameAdd2Cfg(const QString& prefix = "markersave");

public:
    static QString generateCSVFilename(const QString& prefix = "markersave");
public:
    //
    QPointF sceneCenterXY, sceneCenterBL;
    bool isLoaded;
    QString pathCfg;
    QString pathCSV;
    QDir wdir;
    QString mapName, currentZoom;
    QStringList listZoom;
    QStringList listObjTypes;
    QMap<QString, QStringList> mapObjSubtypes;
    QVector<ImageMeta> listMetaInfo;
};

class DatasetUtils
{
private:
    DatasetUtils() {}
public:

};

#endif // DATASETUTILS_H
