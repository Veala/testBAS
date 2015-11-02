#include "frame.h"
#include <QDebug>
#include <QMenu>

Cell::Cell(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    resize(100,50);
    QPalette palette;
    palette.setColor(this->backgroundRole(),QColor(Qt::yellow));
    this->setPalette(palette);
    this->setFrameStyle(QFrame::Box | QFrame::Plain);

    parentCell = NULL;
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        dx = event->globalPos().x() - this->x();
        dy = event->globalPos().y() - this->y();
    } else if (event->buttons() == Qt::RightButton) {
        QMenu popupMenu;
        QAction *add = popupMenu.addAction(tr("Добавить узел/ветку"));
        QAction *del = popupMenu.addAction(tr("Удалить узел/ветку"));
        QAction *split = popupMenu.addAction(tr("Отсоединить узел/ветку"));
        popupMenu.exec(event->globalPos());
    }
}

void Cell::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) & (this->parentCell==NULL))
        this->move(event->globalPos().x()-dx, event->globalPos().y()-dy);
}
