#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("OOO_Televox_TV");
//    QCoreApplication::setOrganizationDomain("tlvx.ru");
    QCoreApplication::setApplicationName("Easy_VOLS");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
