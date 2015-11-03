#include "frame.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QWidget w;
//    Cell *c = new Cell(w);
//    c->show();
//    w.show();

    MainWindow w;
    w.show();

    return a.exec();
}
