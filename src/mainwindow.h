#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datasetutils.h"
#include "gui_mapgraphicsscene.h"
#include "gui_mapgraphicsview.h"

class QLabel;

namespace Ui {
class MainWindow;
class MapGraphicsScene;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLabel* labelPosXYlocal;
    QLabel* labelPosXYglobal;
    QLabel* labelPosBL;
    QLabel* labelScaleMPP;

    QString prevPathOpen;

public:
    DatasetConfig config;
    MapGraphicsScene* mapScene;
    void setProgressMinMax(int vmin, int vmax);
    void setProgress(int val);
    void calcGlobalCoordsFromSceneGeoLib(const QPointF& posScene, QPointF& coordXY, QPointF& coordBL);
    void calcGlobalCoordsFromSceneProj4(const QPointF& posScene, QPointF& coordXY, QPointF& coordBL);
    QString getMarkerType() const;
    QString getMarkerSubType() const;
    bool selectDatasetDialog();
    void refreshNumberOfItems();

public slots:
    void slot_showCoordsFromScenePos(const QPointF& pos);
    void slot_showGlobalCoordsFromScenePos(const QPointF& pos);

private slots:
    void on_actionFull_Screen_triggered();

    void on_action_Quit_triggered();

    void on_pushButton_Push_clicked();

    void on_pushButton_LoadDataset_clicked();

    void on_comboBox_Zoom_activated(const QString &arg1);

    void on_comboBox_ObjType_activated(const QString &arg1);

    void on_comboBox_Zoom_currentIndexChanged(const QString &arg1);

    void on_pushButton_Add_clicked();

    void on_pushButton_Remove_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_Dataset_triggered();

    void on_pushButton_SelectDataset_clicked();

private:
    void loadDataset();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
