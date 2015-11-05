#include "frame.h"
#include <QDebug>
#include <QMenu>
#include <QPainter>

Cell::Cell(QWidget *parent) : QFrame(parent)
{
    setAutoFillBackground(true);      //to visible pallete
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);//(Qt::Window | Qt::FramelessWindowHint);
    setFrameStyle(QFrame::Box | QFrame::Plain);
    resize(100,50);
    QHBoxLayout *curLayout = new QHBoxLayout();
    setLayout(curLayout);
    curLayout->setMargin(10);
    curLayout->setSpacing(5);
    curLayout->addWidget(&left_l);  curLayout->addWidget(&name);    curLayout->addWidget(&right_l);
    left_l.setAlignment(Qt::AlignLeft | Qt::AlignVCenter); right_l.setAlignment(Qt::AlignRight | Qt::AlignVCenter);   name.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //QFont *f = new QFont ("Helvetica", 12, 2, true);
    QFont f("Helvetica", 12, 2, true);
    left_l.setFont(f); right_l.setFont(f);    name.setFont(f);

    QPalette palette;
    QBrush br(Qt::yellow);
    palette.setBrush(QPalette::Window, br);
    this->setPalette(palette);

    parentCell = NULL;
}

void Cell::slotAddTree(bool)
{

}

void Cell::split(bool)
{
    if (parentCell == NULL) return;
//    QList<Cell*>::iterator it;
//    it = parentCell->children.begin();
//    while (it != parentCell->children.end()) {
//        if (*it == this) {
//            parentCell->children.erase(it);
//            break;
//        }
//        ++it;
//    }
    int index = parentCell->children.indexOf(this);
    parentCell->children.removeAt(index);
    int n = parentCell->children.size();
    for (int i=0; i<n; i++)
        parentCell->children.at(i)->lineToParent.setP2(QPoint(parentCell->x()+parentCell->width()*(i+1)/(n+1), parentCell->y()+parentCell->height()));

    disconnect(parentCell,SIGNAL(signalMoveChildren(int,int)),this,SLOT(slotMoveChildren(int,int)));
    disconnect(parentCell,SIGNAL(signalDelCell()),this,SLOT(slotDelCell()));
    this->parentCell = NULL;
    emit newTree();
    emit drawLine();

}

void Cell::del(bool b)
{
    if (parentCell != NULL) {
        int index = parentCell->children.indexOf(this);
        parentCell->children.removeAt(index);
        int n = parentCell->children.size();
        for (int i=0; i<n; i++)
            parentCell->children.at(i)->lineToParent.setP2(QPoint(parentCell->x()+parentCell->width()*(i+1)/(n+1), parentCell->y()+parentCell->height()));
    }
    slotDelCell();
}

void Cell::slotDelCell()
{
    emit signalDelCell();
    delete this;
}

void Cell::slotMoveChildren(int dx, int dy)
{
    emit signalMoveChildren(dx, dy);
    lineToParent.setP1(QPoint(lineToParent.x1()+dx, lineToParent.y1()+dy));
    lineToParent.setP2(QPoint(lineToParent.x2()+dx, lineToParent.y2()+dy));
    this->move(this->x()+dx, this->y()+dy);
    emit drawLine();
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        dX = event->globalPos().x() - this->x();
        dY = event->globalPos().y() - this->y();
    } else if (event->buttons() == Qt::RightButton) {
        QMenu popupMenu;
        QAction *addCell = popupMenu.addAction(tr("Добавить узел"));
        QAction *addTree = popupMenu.addAction(tr("Добавить дерево"));
        QAction *split = popupMenu.addAction(tr("Отсоединить"));
        QAction *rename = popupMenu.addAction(tr("Переименовать"));
        QAction *del = popupMenu.addAction(tr("Удалить"));
        //connect(add,SIGNAL(triggered(bool)),this,SLOT(add(bool)));
        connect(addCell,SIGNAL(triggered(bool)),this,SIGNAL(addChild(bool)));
        connect(addTree,SIGNAL(triggered(bool)),this,SIGNAL(addTree(bool)));
        connect(split,SIGNAL(triggered(bool)),this,SLOT(split(bool)));
        connect(rename,SIGNAL(triggered(bool)),this,SIGNAL(rename(bool)));
        connect(del,SIGNAL(triggered(bool)),this,SLOT(del(bool)));
        popupMenu.exec(event->globalPos());
    }
}

void Cell::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        emit signalMoveChildren(event->globalPos().x()-dX-this->x(), event->globalPos().y()-dY-this->y());
        if (parentCell != NULL)
            lineToParent.setP1(QPoint(event->globalPos().x()-dX + width()/2, event->globalPos().y()-dY));
        this->move(event->globalPos().x()-dX, event->globalPos().y()-dY);
        emit drawLine();
    }
}
