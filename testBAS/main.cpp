#include "frame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    Cell *c = new Cell(w);
    c->show();
    w.show();

    return a.exec();
}
