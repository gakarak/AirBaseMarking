#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T16:06:12
#
#-------------------------------------------------

QT       += core gui #positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectAirBaseMarking
TEMPLATE = app

LIBS += -lGeographic
LIBS += -lproj

SOURCES += main.cpp\
        mainwindow.cpp \
    geoutils.cpp \
    hlgutils.cpp \
    datasetutils.cpp \
    gui_mapgraphicsview.cpp \
    gui_mapgraphicsscene.cpp \
    my_itemcorner.cpp \
    my_itemcirculararea.cpp \
    my_itemrectarea.cpp \
    my_markercircular.cpp \
    my_markerrect.cpp \
    my_markerposition.cpp \
    my_markerbasic.cpp

HEADERS  += mainwindow.h \
    geoutils.h \
    hlgutils.h \
    datasetutils.h \
    gui_mapgraphicsview.h \
    gui_mapgraphicsscene.h \
    my_itemcorner.h \
    my_itemcirculararea.h \
    my_itemrectarea.h \
    my_markercircular.h \
    my_markerrect.h \
    my_markerposition.h \
    my_markerbasic.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
