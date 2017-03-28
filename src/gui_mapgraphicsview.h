#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QWidget *parent = 0);
    void wheelEvent(QWheelEvent *ev);
    void keyPressEvent(QKeyEvent* event);
    void zoomIn();
    void zoomOut();
};

#endif // MAPGRAPHICSVIEW_H
