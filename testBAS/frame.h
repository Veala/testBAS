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

    int dx, dy;

public slots:
    void add(bool);
    void del(bool);
    void split(bool);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};


#endif // FRAME_H
