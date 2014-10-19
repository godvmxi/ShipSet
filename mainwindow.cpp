#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();\
    this->tank_1 =  new tank(this,"tanks 1");
    this->tank_2 =  new tank(this,"gedd");
    this->tank_3 =  new tank(this,"ddd");
    this->mainLayout->addWidget(this->tank_1);
    this->mainLayout->addWidget(this->tank_2);

    this->mainLayout->addWidget(this->tank_3);
    ui->centralWidget->setLayout(this->mainLayout);


    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ship.db");

}

MainWindow::~MainWindow()
{
    delete ui;
}
