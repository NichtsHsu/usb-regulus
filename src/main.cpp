#include "mainwindow/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/usb-regulus.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
