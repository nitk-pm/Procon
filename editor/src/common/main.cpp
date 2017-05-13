#include "common/mainwindow.h"
#include <QApplication>
#include <QtCore/QtGlobal>
#include <QtCore/QTime>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    MainWindow w;
    w.show();

    return a.exec();
}
