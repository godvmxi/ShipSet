#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();\

    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ship.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    this->shipInfo = this->sqlCore->queryShipInfo(this->shipId);

    this->widgetTankItems =  new QVector<Tank>(this->shipInfo.tankNumber);

    for(int i = 0 ; i< this->shipInfo.tankNumber;i++)
    {
        int tankId = i /2 + 1;
        QString tankName ;
        Tank temp = (*this->widgetTankItems)[i];

        if(i%2 == 0){
            tankName = QString("左 %1 舱").arg(tankId);
        }
        else {
            tankName = QString("右 %1 舱").arg(tankId);
        }

        temp.setTankName(tankName);
        temp.setTankId(i);
        temp.setShipId(this->shipId);
        this->mainLayout->addWidget(&((*this->widgetTankItems)[i] ));
    }
//    for(int i = 0;i<this->shipInfo.tankNumber;i++){
//        this->mainLayout->addWidget( &((*this->widgetTankItems)[i] ));
//    }

    ui->centralWidget->setLayout(this->mainLayout);

//    this->sqlCore->queryTankInfo(1,2,5);

}

MainWindow::~MainWindow()
{
    delete ui;
}
