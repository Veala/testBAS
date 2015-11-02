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

void Cell::add(bool)
{
    Dialog d;
    for (int i=0; i<=children.size(); i++)
        d.ui->comboBox->addItem(QString::number(i));
    if (d.exec()) {
        Cell* cell = new Cell(0);
        cell->parentCell = this;

        cell->move(pos().x(), pos().y() + 3*height()/2);
        cell->show();
    }
}

void Cell::del(bool)
{

}

void Cell::split(bool)
{

}

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        dx = event->globalPos().x() - this->x();
        dy = event->globalPos().y() - this->y();
    } else if (event->buttons() == Qt::RightButton) {
        QMenu popupMenu;
        QAction *add = popupMenu.addAction(tr("Добавить узел"));
        QAction *del = popupMenu.addAction(tr("Удалить узел/ветку"));
        QAction *split = popupMenu.addAction(tr("Отсоединить узел/ветку"));
        connect(add,SIGNAL(triggered(bool)),this,SLOT(add(bool)));
        connect(del,SIGNAL(triggered(bool)),this,SLOT(del(bool)));
        connect(split,SIGNAL(triggered(bool)),this,SLOT(split(bool)));
        popupMenu.exec(event->globalPos());
    }
}

void Cell::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) & (this->parentCell==NULL))
        this->move(event->globalPos().x()-dx, event->globalPos().y()-dy);
}
