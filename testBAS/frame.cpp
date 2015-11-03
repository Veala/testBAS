#include "frame.h"
#include <QDebug>
#include <QMenu>
#include <QPainter>

Cell::Cell(QWidget *parent) : QFrame(parent)
{
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);//(Qt::Window | Qt::FramelessWindowHint);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    resize(100,50);
    QPalette palette;
    QBrush br(Qt::yellow);
    //palette.setColor(this->backgroundRole(),QColor(Qt::yellow));
    palette.setBrush(QPalette::Window, br);
    this->setPalette(palette);

    parentCell = NULL;
}

void Cell::add(bool)
{
//    Dialog d;
//    for (int i=0; i<=children.size(); i++)
//        d.ui->comboBox->addItem(QString::number(i));
//    if (d.exec()) {
//        Cell* cell = new Cell((QWidget*)this->parent());
//        cell->parentCell = this;
//        children.push_back(cell);
//        cell->move(pos().x(), pos().y() + 3*height()/2);

//        cell->lineToParent.setP1(QPoint(cell->x()+cell->width()/2, cell->y()));
//        cell->lineToParent.setP2(QPoint(x()+width()/2, y()+height()));
//        emit drawLine(cell->lineToParent);
//        connect(this,SIGNAL(signalMoveChildren(int,int)),cell,SLOT(slotMoveChildren(int,int)));

//        cell->show();
//    }
}

void Cell::del(bool)
{

}

void Cell::split(bool)
{

}

void Cell::slotMoveChildren(int dx, int dy)
{
    emit signalMoveChildren(dx, dy);
    lineToParent.setP1(QPoint(lineToParent.x1()+dx, lineToParent.y1()+dy));
    lineToParent.setP2(QPoint(lineToParent.x2()+dx, lineToParent.y2()+dy));
    this->move(this->x()+dx, this->y()+dy);
    emit drawLine();
//    lineToParent.setP1(QPoint(lineToParent.x1()+dx, lineToParent.y1()+dy));
//    lineToParent.setP2(QPoint(lineToParent.x2()+dx, lineToParent.y2()+dy));
//    emit drawLine(lineToParent);
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        dX = event->globalPos().x() - this->x();
        dY = event->globalPos().y() - this->y();
    } else if (event->buttons() == Qt::RightButton) {
        QMenu popupMenu;
        QAction *add = popupMenu.addAction(tr("Добавить узел"));
        QAction *del = popupMenu.addAction(tr("Удалить узел/ветку"));
        QAction *split = popupMenu.addAction(tr("Отсоединить узел/ветку"));
        //connect(add,SIGNAL(triggered(bool)),this,SLOT(add(bool)));
        connect(add,SIGNAL(triggered(bool)),this,SIGNAL(addChild(bool)));
        connect(del,SIGNAL(triggered(bool)),this,SLOT(del(bool)));
        connect(split,SIGNAL(triggered(bool)),this,SLOT(split(bool)));
        popupMenu.exec(event->globalPos());
    }
}

void Cell::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        emit signalMoveChildren(event->globalPos().x()-dX-this->x(), event->globalPos().y()-dY-this->y());
        lineToParent.setP1(QPoint(event->globalPos().x()-dX + width()/2, event->globalPos().y()-dY));
        this->move(event->globalPos().x()-dX, event->globalPos().y()-dY);
        emit drawLine();
    }
}
