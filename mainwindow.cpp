#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"
#include <stdio.h>
#include <string.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();\

    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ships.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    this->shipInfo = this->sqlCore->queryShipInfo(this->shipId);

    QString left = QString::fromUtf8("左 ");
    QString right = QString::fromUtf8("右 ");
    QString room = QString::fromUtf8(" 舱");
    for(int i = 0 ; i< this->shipInfo.tankNumber;i++)
    {
        int tankId = i /2 + 1;
        QString tankName ;
        char name[64];
        Tank *temp = new Tank();
        char numberChar[] = "一二三四五六七八九十";
        char tmp[32] = {0};
        QString index ;
        qDebug()<<strlen(numberChar);
        if(i%2 == 0){
            tankId -= 1;
            memcpy(tmp,numberChar + tankId*3,3);
            index =  QString::fromUtf8(tmp);
            tankName = left + index + room;
        }
        else {
            tankId -= 1;
            memcpy(tmp,numberChar + tankId*3,3);
            index =  QString::fromUtf8(tmp);
            tankName = right + index + room;
//            sprintf(name,"右 %d 舱",tankId);
        }

        temp->setTankName(tankName);
        temp->setTankId(i);
        temp->setShipId(this->shipId);
        this->mainLayout->addWidget(temp);
//        this->widgetTankItems.append(*temp);
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
