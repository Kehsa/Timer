#include <iostream>

#include <QtWidgets/QApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
