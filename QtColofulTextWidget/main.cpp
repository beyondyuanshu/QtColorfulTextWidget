#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QColor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;
    m.show();


    return a.exec();
}
