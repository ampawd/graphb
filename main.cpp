#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.getGraph().loadCoorinateSystem();

    w.show();

    return a.exec();
}
