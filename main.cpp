#include "mainwindow.h"
#include <QApplication>
#include "brower.h"
#include "QGridLayout"
#include "QPushButton"
#include "qwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWin;

    mainWin.show();
    return a.exec();
}
