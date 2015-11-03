#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<QLine*> lines;
private:
    Ui::MainWindow *ui;

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

public slots:
    void addRoot(bool);
    void addChild(bool);
    void newLine(QLine&);
    void drawLine();

};

#endif // MAINWINDOW_H
