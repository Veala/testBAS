#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "frame.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<Cell*> cells;
private:
    Ui::MainWindow *ui;

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

public slots:
    void addRoot(bool);
    void addChild(bool);
    void addTree(bool);
    void newTree();
    void delTree();
    void drawRecursion(Cell*);
    Cell* searchNULL(Cell*);
    void rename(bool);

};

#endif // MAINWINDOW_H
