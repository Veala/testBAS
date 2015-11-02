#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QPaintEvent>

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

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};


#endif // FRAME_H
