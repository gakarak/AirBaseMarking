#include "gui_mapgraphicsscene.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QDebug>

#include <QFile>
#include <QTextStream>

#include <QMessageBox>

#include "mainwindow.h"
#include "my_markerbasic.h"
#include "my_markercircular.h"
#include "my_markerrect.h"

MapGraphicsScene::MapGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->main      = NULL;
    this->marker    = NULL;
}

void MapGraphicsScene::setMainWindow(MainWindow *ptr)
{
    this->main = ptr;
}

DatasetConfig &MapGraphicsScene::cfg()
{
    return this->main->config;
}

void MapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << event->scenePos();
    emit signalSendScenePos(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void MapGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->scenePos();
    if(event->modifiers()==Qt::ControlModifier) {
        QString markerType      = this->main->getMarkerType();
        QString markerSubType   = this->main->getMarkerSubType();
        MarkerBasic* myItem = NULL;
        if(markerType==MapMarkers::TAirplane) {
            myItem = new MarkerCircular();
        } else if(markerType==MapMarkers::TDontCare) {
            myItem = new MarkerRect();
        } else {
            QMessageBox::critical(
                        this->main,
                        "Error",
                        QString("Incorrect type of marker [%1], please, check that data is loaded").arg(markerType));
        }
        if(myItem!=NULL) {
            myItem->setMarkerType(markerType);
            myItem->setMarkerSubType(markerSubType);
            myItem->setPos(event->scenePos());
            this->addItem(myItem);
        }
        main->refreshNumberOfItems();
    } else {
        emit signalSendScenePos2Global(event->scenePos());
        if(this->marker==NULL) {
            this->marker = new MarkerPosition();
        }
        this->marker->setPos(event->scenePos());
        this->addItem(this->marker);
    }
}

void MapGraphicsScene::clearScene()
{
    QList<QGraphicsItem*> lst = items();
    for(int ii=0; ii<lst.size(); ii++) {
        QGraphicsItem* tptr = lst[ii];
        removeItem(tptr);
        delete tptr;
        tptr = NULL;
    }
    //
    this->marker = NULL;
}

void MapGraphicsScene::removeSelectedItems()
{
    QList<QGraphicsItem*> lstSelectedItems = selectedItems();
    foreach (QGraphicsItem* it, lstSelectedItems) {
        int itType = it->type();
        if( (itType==TypeMarkerCircular) ||
            (itType==TypeMarkerRect) ||
            (itType==TypeMarkerPosition)) {
            removeItem(it);
            delete it;
            //TODO: stupid hak:
            if(it==this->marker) {
                this->marker = NULL;
            }
            it = NULL;
        } else {
            qDebug() << "Item [" << itType << "] is not removable, UserItemId = " << QGraphicsItem::UserType;
        }
    }
}

void MapGraphicsScene::exportMarkers2CSV(const QString &pathCSV) const
{
    QList<QGraphicsItem*> lstSelectedItemsExport;
    foreach (QGraphicsItem* it, items()) {
        int itType = it->type();
        if( (itType==TypeMarkerCircular) || (itType==TypeMarkerRect))
        {
            lstSelectedItemsExport.append(it);
        }
    }
    // Check, that list of markers is non-empty
    if(lstSelectedItemsExport.size()>0) {
        QFile fdata(pathCSV);
        if(fdata.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream txt(&fdata);
            QString strCSVHeader = MarkerBasic::getCSVHeader();
            txt << strCSVHeader << "\n";
            foreach (QGraphicsItem* it, lstSelectedItemsExport) {
                   txt << ((MarkerBasic*)it)->exportToCSVString(this->main->config) << "\n";
            }
        }
    } else {
        qDebug() << "Nothing to export...";
    }
}

void MapGraphicsScene::slot_loadImages2Scene()
{
    //TODO: not realised
    this->clearScene();
    const DatasetConfig& cfg = this->cfg();
    cfg.checkIsLoaded();
    QRectF roi    = cfg.getBoundingBoxXYrel(0.0);
    QRectF roiBig = cfg.getBoundingBoxXYrel(0.1);
    this->setSceneRect(roiBig);
    QPointF tpc = roi.center();
    QPen tpen(Qt::green);
    QPen tpenImg(Qt::red);
    tpen.setWidth(9);
    tpenImg.setWidth(3);
    this->addLine(QLineF(QPointF(roiBig.left(), tpc.y()),  QPointF(roiBig.right(), tpc.y()) ), tpen);
    this->addLine(QLineF(QPointF(tpc.x(), roiBig.top()),   QPointF(tpc.x(), roiBig.bottom())), tpen);
    this->addRect(roi,    tpen);
    this->addRect(roiBig, tpen);
    double trad = 30.;
    this->addEllipse(QRectF(-trad,-trad, 2*trad,2*trad), tpen);
    // load Images:
    QString tzoom   = cfg.currentZoom;
    QString tmap    = cfg.mapName;
    int numMeta     = cfg.listMetaInfo.size();
    this->main->setProgressMinMax(0, numMeta);
    for(int ii=0; ii<numMeta; ii++) {
        const ImageMeta& tmeta = cfg.listMetaInfo.at(ii);
        QString pathImg = cfg.wdir.absoluteFilePath(tmeta.pathImg);
        if(true) {
            QPixmap pxm(pathImg);
            double wpx = (double)pxm.width();
            double hpx = (double)pxm.height();
            QGraphicsPixmapItem* itempx = new QGraphicsPixmapItem(pxm);
            itempx->setScale(0.5*(tmeta.getWidth()/wpx + tmeta.getHeight()/hpx));
            itempx->setPos(tmeta.cornUL_XYrel);
            addItem(itempx);
        }
        this->main->setProgress(ii+1);
        QApplication::processEvents();
    }
    for(int ii=0; ii<numMeta; ii++) {
        const ImageMeta& tmeta = cfg.listMetaInfo.at(ii);
        addRect(QRectF(tmeta.cornUL_XYrel,tmeta.cornBR_XYrel),tpenImg);
    }
}

