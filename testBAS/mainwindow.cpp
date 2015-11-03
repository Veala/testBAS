#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"

#include <QMenu>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        QMenu popupMenu;
        QAction *add = popupMenu.addAction(tr("Добавить узел"));
        connect(add,SIGNAL(triggered(bool)),this,SLOT(addRoot(bool)));
        popupMenu.exec(event->globalPos());
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    for (int i=0; i<lines.size(); i++) {
        QPainter p;
        p.begin(this);
        p.setPen(Qt::black);
        p.drawLine(*lines[i]);
        p.end();
    }
}

void MainWindow::addRoot(bool)
{
    Dialog d;
    if (d.exec()) {
        Cell *cell = new Cell(this);
        connect(cell,SIGNAL(addChild(bool)),this,SLOT(addChild(bool)));
        connect(cell,SIGNAL(newLine(QLine&)),this,SLOT(newLine(QLine&)));

        cell->show();
    }
}

void MainWindow::addChild(bool)
{
    Dialog d;
    Cell *sender = (Cell*)this->sender();
    for (int i=0; i<=sender->children.size(); i++)
        d.ui->comboBox->addItem(QString::number(i));
    if (d.exec()) {
        Cell* cell = new Cell(this);
        cell->parentCell = sender;
        sender->children.push_back(cell);
        cell->move(sender->pos().x(), sender->pos().y() + 3*sender->height()/2);

        cell->lineToParent.setP1(QPoint(cell->x()+cell->width()/2, cell->y()));
        cell->lineToParent.setP2(QPoint(sender->x()+sender->width()/2, sender->y()+sender->height()));
        connect(sender,SIGNAL(signalMoveChildren(int,int)),cell,SLOT(slotMoveChildren(int,int)));
        connect(cell,SIGNAL(newLine(QLine&)),this,SLOT(newLine(QLine&)));
        connect(cell,SIGNAL(drawLine()),this,SLOT(drawLine()));
        connect(cell,SIGNAL(addChild(bool)),this,SLOT(addChild(bool)));
        emit cell->newLine(cell->lineToParent);

        cell->show();
    }
}

void MainWindow::newLine(QLine &line)
{
    lines.push_back(&line);
    update();
}

void MainWindow::drawLine()
{
    update();
}
