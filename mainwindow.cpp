#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();



    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ships.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    this->shipInfo = this->sqlCore->queryShipInfo(this->shipId);

    this->addWidgeHeadInfo();
    this->mainLayout->addWidget(this->widgetHeadInfo);

    QString left = QString::fromUtf8("左 ");
    QString right = QString::fromUtf8("右 ");
    QString room = QString::fromUtf8(" 舱");
    char numberChar[] = "一二三四五六七八九十";
    for(int i = 0 ; i< this->shipInfo.tankNumber;i++)
    {
        int tankId = i /2 + 1;
        QString tankName ;
//        char name[64];
        Tank *itemTank = new Tank();
        char tmp[32] = {0};
        QString index ;
//        qDebug()<<strlen(numberChar);
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

        itemTank->setTankName(tankName);
        itemTank->setTankId(i+1);
        itemTank->setShipId(this->shipId);
        this->mainLayout->addWidget(itemTank);
        this->widgetTankItems[i] = (size_t)itemTank;
        //connect

        connect(itemTank,SIGNAL(tryQueryBankInfo(int,int)),this,SLOT(queryTankInfoSlot(int,int)));
    }
    ui->centralWidget->setLayout(this->mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addWidgeHeadInfo(void){


    this->labelCrt = new QLabel();
    this->labelShipName = new QLabel();
    this->labelTrim = new QLabel();
    this->doubleSpinBoxTrim = new QDoubleSpinBox();
    this->labelFinalDate =  new QLabel();
    this->widgetHeadInfo = new QWidget();
    this->hBoxLayoutHeadInfo = new QHBoxLayout();

    this->labelCrt->setText(QString::fromUtf8("证书号 : ")+this->shipInfo.crt);
    this->labelShipName->setText(QString::fromUtf8("船名 : ")+this->shipInfo.shipName);
    this->labelTrim->setText(QString::fromUtf8("纵倾值 : "));
    this->labelTrim->setFixedWidth(60);
    this->doubleSpinBoxTrim->setMinimum(this->shipInfo.shipTrimMin);
    this->doubleSpinBoxTrim->setSingleStep(this->shipInfo.shipTrimStep);
    this->doubleSpinBoxTrim->setMaximum(this->shipInfo.shipTrimMin + this->shipInfo.shipTrimStep*this->shipInfo.capacityNumber);
    this->doubleSpinBoxTrim->setFixedWidth(60);
    this->doubleSpinBoxTrim->setValue(this->shipInfo.shipTrimMin);
    this->labelFinalDate->setText(QString::fromUtf8("有效期 : ")+this->shipInfo.finalDate.toString());

    this->shipTrimMax = this->shipInfo.shipTrimMin +  this->shipInfo.shipTrimStep * (this->shipInfo.capacityNumber - 1) ;

    this->hBoxLayoutHeadInfo->addWidget(this->labelCrt);
    this->hBoxLayoutHeadInfo->addWidget(this->labelShipName);
    this->hBoxLayoutHeadInfo->addWidget(this->labelTrim);
    this->hBoxLayoutHeadInfo->addWidget(this->doubleSpinBoxTrim);
    this->hBoxLayoutHeadInfo->addWidget(this->labelFinalDate);

    this->widgetHeadInfo->setLayout(this->hBoxLayoutHeadInfo);
    connect(this->doubleSpinBoxTrim,SIGNAL(valueChanged(QString)),this,SLOT(shipTrimChanged(QString)) );



}
void MainWindow::updateWidgetTankTrim(void){

}

void MainWindow::queryTankInfoSlot(int tankId,int sounding)
{
    float capacity = 0;
    if(sounding%10 != 0){
        int temp = sounding %10 ;
        int sounding_1 =  sounding -  temp;
        int sounding_2 =  sounding_1 +10;
        qDebug()<<"will query two sounding -> "<<tankId << sounding_1 << sounding_2;
        float capacity_1 = this->queryTankCapacity(tankId,sounding_1);
        float capacity_2 = this->queryTankCapacity(tankId,sounding_2);
        capacity = (capacity_2 - capacity_1 ) /10 * temp + capacity_1 ;
        qDebug()<<capacity <<capacity_1<<capacity_2;
    }
    else {
        capacity = this->queryTankCapacity(tankId,sounding);
    }
    Tank *tank =  (Tank *)this->widgetTankItems[tankId - 1];
    //temprature modify

    tank->setTankCapacity(capacity);


}
float  MainWindow::queryTankCapacity(int tankId,int sounding)
{

    TankInfo info = this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding);
    if((info.tankId != tankId ) || (info.shipId != this->shipInfo.shipId) ){
        qDebug()<<"query bank error";
        return -1;
    }
    //cal trim .add soon
    float temp= (this->currentShipTrim -this->shipInfo.shipTrimMin );
    int trimFlag = -1; // 0:正好整除, 1:介于二者之间,具体查看min ~max
    float min = 0;
    float max;
    int i =0;
    for(i=0;i<this->shipInfo.capacityNumber;i++){
        min = this->shipInfo.shipTrimMin + i * this->shipInfo.shipTrimStep ;
        max = min + this->shipInfo.shipTrimStep ;
        if((this->currentShipTrim - min ) < 0.001){
            trimFlag = 0;
            break;
        }
        else {
            if (this->currentShipTrim < max){
                trimFlag = 1;
                break;
            }

        }
    }
    if(trimFlag == 0){
        qDebug()<<"just ok";
        return info.capacity[i];
    }
    else if(trimFlag == 1){
        qDebug()<<"not just ok ->  "<< this->currentShipTrim <<min << max;
        float capacity_1 =  info.capacity[i];
        float capacity_2 =  info.capacity[i+1];
        return    ( (capacity_2 -capacity_1 ) /this->shipInfo.shipTrimStep )*(this->currentShipTrim -min)  + capacity_1 ;
    }
    else
        return info.capacity[0];
}
void MainWindow::shipTrimChanged(QString d){

    float trim = d.toFloat();
    if((trim < this->shipInfo.shipTrimMin) || (trim >this->shipTrimMax )){
        this->doubleSpinBoxTrim->setValue(this->currentShipTrim);
        return ;
    }
    this->currentShipTrim = trim;
//    qDebug()<<"ship trim changed ->" <<d  << this->currentShipTrim;
}
