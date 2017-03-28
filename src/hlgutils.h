#ifndef HLGUTILS_H
#define HLGUTILS_H

#include <QString>
#include <QPointF>

class HLGData {
public:
    HLGData(const QString& path) {
        this->path = path;
    }
    void    loadHLG();
    QPointF getCenterBL() const;
    QString toString(bool isInKm=false) const;
    //
    QString path;
    QPointF pts0, pts1, pts2, pts3;
    int zoom;
};


class HLGUtils
{
private:
    HLGUtils();
public:
    static HLGData loadHLGData(const QString& path);
    static HLGData project2XY(const HLGData& hlg, bool isCentered=true);
};

#endif // HLGUTILS_H
