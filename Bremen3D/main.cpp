//#include "widget.h"
#include "defines.h"
#include "Mainwindow.h"
#include "graphwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qApp->addLibraryPath(".\libs");
    qApp->setOrganizationName("Crazy Horse");
    qApp->setApplicationVersion(VERSION);
    MainWindow w;
    w.show();
    return app.exec();
}
