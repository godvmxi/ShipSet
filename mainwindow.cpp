#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();\


    ui->centralWidget->setLayout(this->mainLayout);


    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ship.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    this->sqlCore->queryShipInfo(0);
//    this->sqlCore->queryTankInfo(1,2,5);

}

MainWindow::~MainWindow()
{
    delete ui;
}
