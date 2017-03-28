#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>

#include <QGraphicsItemGroup>

#include "datasetutils.h"
#include "my_markerposition.h"

class MainWindow;
class QGraphicsSceneMouseEvent;

class MapGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapGraphicsScene(QObject *parent = 0);
    void setMainWindow(MainWindow *ptr = 0);
    DatasetConfig& cfg();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void clearScene();
    void removeSelectedItems();
    void exportMarkers2CSV(const QString& pathCSV) const;
    //
signals:
    void signalSendScenePos(const QPointF& pos);
    void signalSendScenePos2Global(const QPointF& pos);
public slots:
    void slot_loadImages2Scene();
    //
public:
    MainWindow* main;
private:
    MarkerPosition* marker;
};

#endif // MAPGRAPHICSSCENE_H
