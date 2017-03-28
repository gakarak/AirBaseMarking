#include "gui_mapgraphicsview.h"

#include "gui_mapgraphicsscene.h"

#include <QDebug>

MapGraphicsView::MapGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{

}

void MapGraphicsView::wheelEvent(QWheelEvent *ev)
{
    MapGraphicsScene* myScene = (MapGraphicsScene*)scene();
    Q_UNUSED(myScene)
    if(ev->modifiers()==Qt::ControlModifier) {
        if(ev->delta()>0) {
            zoomIn();
            qDebug() << "ZoomIn()";
        } else {
            zoomOut();
            qDebug() << "ZoomOut()";
        }
    } else {
        QGraphicsView::wheelEvent(ev);
    }
}

void MapGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if( (event->key() == Qt::Key_Delete) || (event->key()==Qt::Key_D)) {
        MapGraphicsScene* myScene = (MapGraphicsScene*)scene();
        myScene->removeSelectedItems();
    }
    qDebug() << "Key is: " << event->key();
}

void MapGraphicsView::zoomIn()
{
    scale(1.1, 1.1);
}

void MapGraphicsView::zoomOut()
{
    scale(0.9,0.9);
}
