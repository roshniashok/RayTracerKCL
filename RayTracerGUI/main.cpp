#include "raytracingg.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RayTracingg w;
    w.show();

    return a.exec();
}
