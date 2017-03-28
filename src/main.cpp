#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include <QVector>

struct MyClass {
    MyClass(int px, int py) {
        this->x = px;
        this->y = py;
    }
    ~MyClass() {}
    int x,y;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
