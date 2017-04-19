#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFrame>
#include <QLabel>

#include "geoutils.h"
#include "hlgutils.h"
#include "datasetutils.h"

#include <QSpacerItem>
#include <QRectF>
#include <QLineF>
#include <QPainter>

#include <QMessageBox>

#include "my_markercircular.h"
#include "my_itemcorner.h"
#include "my_itemcirculararea.h"

#include "my_markerrect.h"
#include "my_itemrectarea.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    this->prevPathOpen = QDir::homePath();
    //
    mapScene = new MapGraphicsScene(this);
    mapScene->setMainWindow(this);
    ui->graphicsView->setScene(mapScene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    //
    QRectF rect = QRectF(-1000., -1000., 2000., 2000.);
    mapScene->setSceneRect(rect);
    //
    this->labelPosXYlocal   = new QLabel("", this);
    this->labelPosXYglobal  = new QLabel("", this);
    this->labelPosBL        = new QLabel("", this);
    this->labelScaleMPP     = new QLabel("", this);
    this->labelPosXYlocal->setFrameStyle(QFrame::StyledPanel);
    this->labelPosXYglobal->setFrameStyle(QFrame::StyledPanel);
    this->labelPosBL->setFrameStyle(QFrame::StyledPanel);
    this->labelScaleMPP->setFrameStyle(QFrame::StyledPanel);
    ui->statusBar->addWidget(this->labelPosXYlocal);
    ui->statusBar->addWidget(this->labelPosXYglobal);

//    QFrame *lineH = new QFrame(this);
//    lineH->setFrameShape(QFrame::VLine);
//    ui->statusBar->addWidget(lineH);
    ui->statusBar->addPermanentWidget(this->labelPosBL);
    ui->statusBar->addPermanentWidget(this->labelScaleMPP);
//    mapScene->addLine(QLineF(rect.topLeft(), rect.bottomRight()));
//    mapScene->addLine(QLineF(rect.bottomLeft(), rect.topRight()));
    QApplication::connect(this->mapScene, SIGNAL(signalSendScenePos(QPointF)), this, SLOT(slot_showCoordsFromScenePos(QPointF)));
    QApplication::connect(this->mapScene, SIGNAL(signalSendScenePos2Global(QPointF)), this, SLOT(slot_showGlobalCoordsFromScenePos(QPointF)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProgressMinMax(int vmin, int vmax)
{
    ui->progressBar->setMinimum(vmin);
    ui->progressBar->setMaximum(vmax);
}

void MainWindow::setProgress(int val)
{
    ui->progressBar->setValue(val);
}

void MainWindow::calcGlobalCoordsFromSceneGeoLib(const QPointF &posScene,
                                                 QPointF &coordXY,
                                                 QPointF &coordBL)
{
    if(this->config.isLoaded) {
        GeoUtils::calcGlobalCoordsFromSceneGeoLib(this->config, posScene,
                                                  coordXY, coordBL);
    }/* else {
        QMessageBox::critical(this,
                              "Error",
                              QString("Please, load dataset before manipulation with BL-coords"));
    }*/
}

void MainWindow::calcGlobalCoordsFromSceneProj4(const QPointF &posScene,
                                                QPointF &coordXY,
                                                QPointF &coordBL)
{
    if(this->config.isLoaded) {
        GeoUtils::calcGlobalCoordsFromSceneProj4(this->config, posScene,
                                                  coordXY, coordBL);
    } /*else {
        QMessageBox::critical(this,
                              "Error",
                              QString("Please, load dataset before manipulation with BL-coords"));
    }*/
}

QString MainWindow::getMarkerType() const {
    return ui->comboBox_ObjType->currentText();
}

QString MainWindow::getMarkerSubType() const
{
    return ui->comboBox_ObjSubtype->currentText();
}

bool MainWindow::selectDatasetDialog()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open dataset-config[info.cfg]"),
                                                    this->prevPathOpen,
                                                    tr("Dataset config (info.cfg)"));
    QFileInfo finf(filename);
    if(finf.exists()) {
        this->prevPathOpen = finf.absoluteDir().absolutePath();
        ui->lineEdit_PathConfig->setText(finf.absoluteFilePath());
        return true;
    }
    return false;
}

void MainWindow::refreshNumberOfItems()
{
    ui->label_NumAirplanes->setText("0");
    ui->label_NumDontcare->setText("0");
    ui->label_NumTotal->setText("0");
    if(this->config.isLoaded) {
        int numAirplanes = 0;
        int numDontcare  = 0;
        int numTotal     = 0;
        foreach (QGraphicsItem *item, mapScene->items()) {
            if(item->type()==TypeMarkerCircular) {
                numAirplanes++;
            }
            if(item->type()==TypeMarkerRect) {
                numDontcare++;
            }
        }
        numTotal = numAirplanes + numDontcare;
        //
        ui->label_NumAirplanes->setText(QString::number(numAirplanes));
        ui->label_NumDontcare->setText(QString::number(numDontcare));
        ui->label_NumTotal->setText(QString::number(numTotal));
    }
}

void MainWindow::slot_showCoordsFromScenePos(const QPointF &pos)
{
    QPointF pxy,pbl;
//    this->calcGlobalCoordsFromSceneGeoLib(pos, pxy, pbl);
    this->calcGlobalCoordsFromSceneProj4(pos, pxy, pbl);
    QString strPosXYlocal   = QString("X: %1, Y: %2").arg(pos.x(),0,'g',8).arg(-pos.y(),0,'g',8);
    QString strPosXYglobal  = QString("Xglob: %1, Yglob: %2").arg(pxy.x(), 0,'g',10).arg(pxy.y(),0,'g',10);
    QString strPosBL        = QString("Lon: %1°, Lat: %2°").arg(pbl.x(),0,'g',16).arg(pbl.y(),0,'g',16);
    this->labelPosXYlocal->setText(strPosXYlocal);
    this->labelPosXYglobal->setText(strPosXYglobal);
    this->labelPosBL->setText(strPosBL);
}

void MainWindow::slot_showGlobalCoordsFromScenePos(const QPointF &pos)
{
    Q_UNUSED(pos)
    QPointF pxy,pbl;
//    this->calcGlobalCoordsFromSceneGeoLib(pos, pxy, pbl);
    this->calcGlobalCoordsFromSceneProj4(pos, pxy, pbl);
    ui->lineEdit_GlobX->setText(QString("%1").arg(pxy.x(),0,'g',16));
    ui->lineEdit_GlobY->setText(QString("%1").arg(pxy.y(),0,'g',16));
    ui->lineEdit_GlobLon->setText(QString("%1").arg(pbl.x(),0,'g',18));
    ui->lineEdit_GlobLat->setText(QString("%1").arg(pbl.y(),0,'g',18));
}

void MainWindow::on_actionFull_Screen_triggered()
{
    if(!isFullScreen()) {
        showFullScreen();
    } else {
        showNormal();
    }
}

void MainWindow::on_action_Quit_triggered()
{
    qApp->quit();
}

void MainWindow::on_pushButton_Push_clicked()
{
    QStringList lst;
    lst << "q1" << "a1" << "a2";
    lst << "q2";
    qDebug() << lst.join("|");
    /*
    QString pathHLG = "/home/ar/data/Data_AirDatabases/datasets/airbase_16/airbase_16.hlg";
    HLGData hlg = HLGUtils::loadHLGData(pathHLG);

    HLGData hlgXY = HLGUtils::project2XY(hlg);
    qDebug() << "BL : " << hlg.toString();
    qDebug() << "XY : " << hlgXY.toString();
    double mppHLP = GeoUtils::getMPPForLatituze(hlg.getCenterBL().y(), hlg.zoom);
    qDebug() << "Zoom(" << hlg.zoom << "), pts = " << hlg.getCenterBL() << ", mpp = " << mppHLP;
    //
    const Geodesic& geod = Geodesic::WGS84;
    double dst_01, dst_12, dst_23, dst_30;
    geod.Inverse(hlg.pts0.y(), hlg.pts0.x(),
                 hlg.pts1.y(), hlg.pts1.x(),dst_01);
    geod.Inverse(hlg.pts1.y(), hlg.pts1.x(),
                 hlg.pts2.y(), hlg.pts2.x(),dst_12);
    geod.Inverse(hlg.pts2.y(), hlg.pts2.x(),
                 hlg.pts3.y(), hlg.pts3.x(),dst_23);
    geod.Inverse(hlg.pts3.y(), hlg.pts3.x(),
                 hlg.pts0.y(), hlg.pts0.x(),dst_30);
    qDebug() << ""
             << "d(0,1) = " << dst_01/1000. << "(km)\n"
             << "d(1,2) = " << dst_12/1000. << "(km)\n"
             << "d(2,3) = " << dst_23/1000. << "(km)\n"
             << "d(3,0) = " << dst_30/1000. << "(km)\n";
    */
}

void MainWindow::on_pushButton_LoadDataset_clicked()
{
    QString txt = this->ui->lineEdit_PathConfig->text();
    QFileInfo finf(txt);
    if(finf.exists()) {
        this->config = DatasetConfig(txt);
        this->config.loadConfig();
        this->ui->comboBox_Zoom->clear();
        this->ui->comboBox_Zoom->addItems(this->config.listZoom);
        this->ui->comboBox_Zoom->setCurrentText(this->config.currentZoom);
        this->ui->comboBox_ObjType->clear();
        this->ui->comboBox_ObjSubtype->clear();
        this->ui->comboBox_ObjType->addItems(this->config.listObjTypes);
        this->ui->comboBox_ObjSubtype->addItems(this->config.mapObjSubtypes.value(this->ui->comboBox_ObjType->currentText()));
        loadDataset();
        refreshNumberOfItems();
    } else {
        QMessageBox::critical(this, "Error", QString("Cant find config fiel [%1]").arg(txt));
    }
}

void MainWindow::on_comboBox_Zoom_activated(const QString &arg1)
{
    this->config.currentZoom = arg1;
    this->config.setZoom();
//    this->on_pushButton_LoadDataset_clicked();
//    this->mapScene->slot_loadImages2Scene();
    loadDataset();
    qDebug() << "Current Zoom: " << this->config.currentZoom;
}

void MainWindow::on_comboBox_ObjType_activated(const QString &arg1)
{
    ui->comboBox_ObjSubtype->clear();
    ui->comboBox_ObjSubtype->addItems(this->config.mapObjSubtypes.value(arg1));
}

void MainWindow::on_comboBox_Zoom_currentIndexChanged(const QString &arg1)
{
    int zoom = arg1.right(arg1.size()-1).toInt();
    if( (zoom>0) && (this->config.isLoaded)) {
        double lat = this->config.sceneCenterBL.y();
        QString strScale = QString("%1 mpp").arg(GeoUtils::getMPPForLatituze(lat,zoom));
        this->labelScaleMPP->setText(strScale);
    }

}

void MainWindow::on_pushButton_Add_clicked()
{
    qDebug() << "FUCK";
    MarkerRect* myItem  = new MarkerRect();
    this->mapScene->addItem(myItem);
}

void MainWindow::on_pushButton_Remove_clicked()
{
    this->mapScene->removeSelectedItems();
}

void MainWindow::on_actionSave_triggered()
{
    if(!config.isLoaded) {
        QMessageBox::critical(this, "Error", "You must load dataset before save markers!");
    } else {
        qDebug() << "Action::Save()";
        this->config.generateCSVFilenameAdd2Cfg();
        mapScene->exportMarkers2CSV(this->config.pathCSV);
    }

}

void MainWindow::on_actionLoad_Dataset_triggered()
{
    if(selectDatasetDialog()) {
        on_pushButton_LoadDataset_clicked();
    }
}

void MainWindow::on_pushButton_SelectDataset_clicked()
{
    this->selectDatasetDialog();
}

void MainWindow::loadDataset()
{
    // 1. Load image background
    this->ui->labelProgress->setText("Load images...");
    this->mapScene->slot_loadImages2Scene();
    // 2. Load saved markers
    QFileInfo finfoMarkers(this->config.pathCSV);
    if(finfoMarkers.exists()) {
        this->ui->labelProgress->setText("Load markers...");
        this->ui->progressBar->setMinimum(0);
        this->ui->progressBar->setMaximum(0);
        this->ui->progressBar->setValue(0);
        QApplication::processEvents();
        QFile fdata(finfoMarkers.absoluteFilePath());
        if(fdata.open(QFile::ReadOnly)) {
            QTextStream txt(&fdata);
            QString strCSV;
            //while (txt.readLineInto(&strCSV)) {
            while (!txt.atEnd()) {
                strCSV = txt.readLine();
                int markerType = MarkerBasic::getTypeMarkerFromCSVString(strCSV);
                MarkerBasic* newMarker = NULL;
                if(markerType==TypeMarkerCircular) {
                    newMarker = new MarkerCircular();
                } else if(markerType==TypeMarkerRect) {
                    newMarker = new MarkerRect();
                } else {
                    qDebug() << "WARNING: unknown marker type skip... [" << strCSV << "]";
                }
                if(newMarker!=NULL) {
                    newMarker->loadFromCSVString(strCSV);
                    newMarker->placeOnScene(this->mapScene);
                }
            }
        }
    }
    // 3. finish
    this->ui->labelProgress->setText("...");
    this->ui->progressBar->setMaximum(1);
}
