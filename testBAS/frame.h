#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QDebug>
#include <QMenu>
#include <QPainter>
#include <dialog.h>

class Cell : public QFrame
{
    Q_OBJECT

public:
    Cell(QWidget *parent=0);

    int left_i;
    int right_i;
    QLabel left_l;
    QLabel right_l;
    QLabel name;
    Cell* parentCell;
    QList<Cell*> children;

    int dX, dY;
    QLine lineToParent;
    void toMe(int&);
    void fromChild(int&, Cell*);

signals:
    void newTree();
    void signalMoveChildren(int,int);
    void delTree();
    void signalDelCell();
    void drawLine();
    void addChild(bool);
    void addTree(bool);
    void rename(bool);

public slots:
    void split(bool);
    void del(bool);
    void slotDelCell();
    void slotMoveChildren(int,int);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};


#endif // FRAME_H
