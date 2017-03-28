# AirBaseMarking
Simple application (with Qt UI interface) for airplane marking program with geo-referenced coordinates.

Dependency:
- [Qt5](https://www.qt.io/)
- [GeographicLib](https://geographiclib.sourceforge.io/)
- [Proj4](http://proj4.org/)

------------------------------

Build and run:
```
mkdir build
cd build
qmake ..
make -j6
./ProjectAirBaseMarking
```

Install dependencies (Ubuntu):
```
sudo apt-get install libgeographiclib-dev libproj-dev
```

![alt tag](img/app-screen-1.png)
![alt tag](img/app-screen-2.png)
![alt tag](img/app-screen-3.png)
