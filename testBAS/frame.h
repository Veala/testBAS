#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QPaintEvent>
#include <dialog.h>

class Cell : public QFrame
{
    Q_OBJECT

public:
    Cell(QWidget *parent=0);

    int left;
    int right;
    Cell* parentCell;
    QList<Cell*> children;

    int dX, dY;
    QLine lineToParent;

signals:
    void newTree();
    void signalMoveChildren(int,int);
    void delTree();
    void signalDelCell();
    void drawLine();
    void addChild(bool);
    void addTree(bool);

public slots:
    void slotAddTree(bool);
    void split(bool);
    void del(bool);
    void slotDelCell();
    void slotMoveChildren(int,int);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};


#endif // FRAME_H
