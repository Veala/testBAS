#include "mainwindow.h"
#include "ui_mainwindow.h"

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
        QAction *add = popupMenu.addAction(tr("Добавить дерево"));
        connect(add,SIGNAL(triggered(bool)),this,SLOT(addRoot(bool)));
        popupMenu.exec(event->globalPos());
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    for (int i=0; i<cells.size(); i++)
        drawRecursion(cells.at(i));
}

void MainWindow::addRoot(bool)
{
    Dialog d;
    d.ui->lineEdit->setEnabled(true);
    d.ui->comboBox->setEnabled(false);
    d.ui->comboBox_tree->setEnabled(false);
    if (d.exec()) {
        Cell *cell = new Cell(this);
        cell->name.setText(d.ui->lineEdit->text());
        connect(cell,SIGNAL(addChild(bool)),this,SLOT(addChild(bool)));
        connect(cell,SIGNAL(signalDelCell()),this,SLOT(delTree()));
        connect(cell,SIGNAL(addTree(bool)),this,SLOT(addTree(bool)));
        connect(cell,SIGNAL(drawLine()),this,SLOT(update()));
        connect(cell,SIGNAL(newTree()),this,SLOT(newTree()));
        connect(cell,SIGNAL(rename(bool)),this,SLOT(rename(bool)));
        cells.push_back(cell);
        cell->show();

//        //-------------------------------------------------Через рекурсию
//        int i=1;
//        leftrightRecursion(NULL,cell,i,"l");
//        //-------------------------------------------------Через рекурсию
        //---------------------------------------------------------Рассчет
        int i=0;
        cell->toMe(i);
        //---------------------------------------------------------Рассчет
    }
}

void MainWindow::addChild(bool)
{
    Dialog d;
    d.ui->lineEdit->setEnabled(true);
    d.ui->comboBox->setEnabled(true);
    d.ui->comboBox_tree->setEnabled(false);
    Cell *sender = (Cell*)this->sender();
    for (int i=0; i<=sender->children.size(); i++)
        d.ui->comboBox->addItem(QString::number(i));
    if (d.exec()) {
        Cell* cell = new Cell(this);
        cell->parentCell = sender;
        sender->children.insert(d.ui->comboBox->currentIndex(), cell);
        cell->move(sender->pos().x(), sender->pos().y() + 3*sender->height()/2);

        int n = sender->children.size();
        cell->lineToParent.setP1(QPoint(cell->x()+cell->width()/2, cell->y()));
        for (int i=0; i<n; i++)
            sender->children.at(i)->lineToParent.setP2(QPoint(sender->x()+sender->width()*(i+1)/(n+1), sender->y()+sender->height()));

        cell->name.setText(d.ui->lineEdit->text());
        connect(sender,SIGNAL(signalMoveChildren(int,int)),cell,SLOT(slotMoveChildren(int,int)));
        connect(sender,SIGNAL(signalDelCell()),cell,SLOT(slotDelCell()));
        connect(sender,SIGNAL(signalDelCell()),this,SLOT(delTree()));
        connect(cell,SIGNAL(drawLine()),this,SLOT(update()));
        connect(cell,SIGNAL(addChild(bool)),this,SLOT(addChild(bool)));
        connect(cell,SIGNAL(signalDelCell()),this,SLOT(delTree()));
        connect(cell,SIGNAL(newTree()),this,SLOT(newTree()));
        connect(cell,SIGNAL(addTree(bool)),this,SLOT(addTree(bool)));
        connect(cell,SIGNAL(rename(bool)),this,SLOT(rename(bool)));
        emit cell->drawLine();

        cell->show();

//        //-------------------------------------------------Через рекурсию
//        int index = cell->parentCell->children.indexOf(cell);
//        int i;
//        if (index == 0) i = cell->parentCell->left_l.text().toInt() + 1;
//        else i = cell->parentCell->children.at(index-1)->right_l.text().toInt() + 1;
//        leftrightRecursion(NULL,cell,i,"l");
//        //-------------------------------------------------Через рекурсию

        //---------------------------------------------------------Рассчет
        int index = cell->parentCell->children.indexOf(cell);
        int i;
        if (index == 0) i = cell->parentCell->left_l.text().toInt();
        else i = cell->parentCell->children.at(index-1)->right_l.text().toInt();
        cell->toMe(i);
        //---------------------------------------------------------Рассчет
    }
}

void MainWindow::addTree(bool)
{
    Dialog d;
    d.ui->lineEdit->setEnabled(false);
    d.ui->comboBox_tree->setEnabled(true);
    d.ui->comboBox->setEnabled(true);
    Cell *sender = (Cell*)this->sender();
    Cell *curParent = searchNULL(sender);
    for (int i=0; i<=sender->children.size(); i++)
        d.ui->comboBox->addItem(QString::number(i));
    for (int i=0; i<cells.size(); i++)
        d.ui->comboBox_tree->addItem(cells.at(i)->name.text());
    if (d.exec()) {
        if (curParent == cells.at(d.ui->comboBox_tree->currentIndex())) {
            QMessageBox::warning(this, "Ошибка", "Выбрано дерево элемента");
            return;
        }
        //Cell *cell = const_cast<Cell*>(cells.at(i));
        Cell *cell = cells.at(d.ui->comboBox_tree->currentIndex());
        cell->parentCell = sender;
        sender->children.insert(d.ui->comboBox->currentIndex(), cell);

        //int index = cells.indexOf(cell);
        cells.removeAt(cells.indexOf(cell));
        //cell->move(sender->pos().x(), sender->pos().y() + 3*sender->height()/2);

        int n = sender->children.size();
        cell->lineToParent.setP1(QPoint(cell->x()+cell->width()/2, cell->y()));
        for (int i=0; i<n; i++)
            sender->children.at(i)->lineToParent.setP2(QPoint(sender->x()+sender->width()*(i+1)/(n+1), sender->y()+sender->height()));
        connect(sender,SIGNAL(signalMoveChildren(int,int)),cell,SLOT(slotMoveChildren(int,int)));
        connect(sender,SIGNAL(signalDelCell()),cell,SLOT(slotDelCell()));
        emit cell->drawLine();

//        //-------------------------------------------------Через рекурсию
//        int index = cell->parentCell->children.indexOf(cell);
//        int i;
//        if (index == 0) i = cell->parentCell->left_l.text().toInt() + 1;
//        else i = cell->parentCell->children.at(index-1)->right_l.text().toInt() + 1;
//        leftrightRecursion(NULL,cell,i,"l");
//        //-------------------------------------------------Через рекурсию

        //---------------------------------------------------------Рассчет
        int index = cell->parentCell->children.indexOf(cell);
        int i;
        if (index == 0) i = cell->parentCell->left_l.text().toInt();
        else i = cell->parentCell->children.at(index-1)->right_l.text().toInt();
        cell->toMe(i);
        //---------------------------------------------------------Рассчет
    }
}

void MainWindow::newTree()
{
    Cell *cell = (Cell *)this->sender();
    cells.push_back(cell);

//    //-------------------------------------------------Через рекурсию
//    int i=1;
//    leftrightRecursion(NULL,cell,i,"l");
//    //-------------------------------------------------Через рекурсию
}

void MainWindow::delTree()
{
    Cell *cell = (Cell *)this->sender();
    if (cell->parentCell == NULL) {
        int index = cells.indexOf(cell);
        if (index != -1) cells.removeAt(index);
    }
    update();
}

void MainWindow::drawRecursion(Cell* cell)
{
    if (cell->parentCell != NULL) {
        QPainter p;
        p.begin(this);
        p.setPen(Qt::black);
        p.drawLine(cell->lineToParent);
        p.end();
    }
    if (cell->children.size() != 0)
        foreach (Cell* nextCell, cell->children)
            drawRecursion(nextCell);
}

//void MainWindow::leftrightRecursion(Cell *end, Cell *cur, int &i, QString lr)
//{
//    if (lr == "l") {
//        cur->left_l.setText(QString::number(i));
//        i++;
//        if (cur->children.size() == 0)  leftrightRecursion(end, cur, i, "r");
//        else    leftrightRecursion(end, cur->children.at(0), i, "l");
//    } else if (lr == "r") {
//        cur->right_l.setText(QString::number(i));
//        if (cur->parentCell == NULL) return;
//        int index = cur->parentCell->children.indexOf(cur);
//        Cell *next;
//        i++;
//        if (index+1 == cur->parentCell->children.size()) {
//            next = cur->parentCell;
//            if (next == end) return;
//            leftrightRecursion(end, next, i, "r");
//        }   else {
//            next = cur->parentCell->children.at(index+1);
//            if (next == end) return;
//            leftrightRecursion(end, next, i, "l");
//        }
//    }
//}

Cell *MainWindow::searchNULL(Cell *cell)
{
    if (cell->parentCell == NULL) return cell;
    else return searchNULL(cell->parentCell);
}

void MainWindow::rename(bool)
{
    Dialog d;
    d.ui->lineEdit->setEnabled(true);
    d.ui->comboBox_tree->setEnabled(false);
    d.ui->comboBox->setEnabled(false);
    Cell *cell = (Cell *)this->sender();
    if (d.exec()) cell->name.setText(d.ui->lineEdit->text());
}
