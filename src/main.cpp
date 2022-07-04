#include "mainwindow.h"

#include <QApplication>
#include "utility.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IconSet::load();

    MainWindow w;
    w.show();
    return a.exec();
}
