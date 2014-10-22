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
    this->setFixedWidth(480);
    this->setFixedHeight(500);



    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ships.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    if((this->shipNumber <= 0 )||(this->shipNumber > MAX_SHIP_NUMBER)){
        QMessageBox::critical(NULL, QString::fromUtf8("船只数据错误"), QString::fromUtf8("船只信息需要0~20--> ")+QString("%1 ").arg(this->shipNumber), QMessageBox::Yes, QMessageBox::Yes);
        return ;
    }
    this->sqlCore->queryShipsInfo(this->shipArray);

    for(int i = 0 ;i<this->shipNumber;i++){
        qDebug()<<"shipName --> "<<this->shipArray[i].shipName;
    }
    this->shipInfo = this->shipArray[0];





    this->addWidgeHeadInfo();
    this->mainLayout->addWidget(this->widgetHeadInfo);
    this->labelTableTitle = new QLabel();
    this->labelTableTitle->setText(QString::fromUtf8("     舱     室       测深高度          舱室温度                                  舱室容量"));
    this->mainLayout->addWidget(this->labelTableTitle);

    this->widgetTankItemsTable = new QWidget();
    this->widgetTankItemsTable->setFixedWidth(440);
    this->vBoxLayoutTankItemsTable = new QVBoxLayout();
    this->scrollAreaTankItemsTable = new QScrollArea();
    this->scrollAreaTankItemsTable->setFixedWidth(460);

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
        this->vBoxLayoutTankItemsTable->addWidget(itemTank);
        this->widgetTankItems[i] = (size_t)itemTank;
        //connect

        connect(itemTank,SIGNAL(tryQueryBankInfo(int,int)),this,SLOT(queryTankInfoSlot(int,int)));
    }
    this->widgetTankItemsTable->setLayout(this->vBoxLayoutTankItemsTable);
    this->scrollAreaTankItemsTable->setWidget(this->widgetTankItemsTable);
    this->mainLayout->addWidget(this->scrollAreaTankItemsTable);

    this->addWidgeFootInfo();
    this->mainLayout->addWidget(this->widgetFootInfo);
    ui->centralWidget->setLayout(this->mainLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addWidgeHeadInfo(void){


    this->labelCrt = new QLabel();
    this->labelShipName = new QLabel();
    this->comboBoxShipCrt =  new QComboBox();
    this->labelTrim = new QLabel();
    this->doubleSpinBoxTrim = new QDoubleSpinBox();

    this->widgetHeadInfo = new QWidget();
    this->hBoxLayoutHeadInfo = new QHBoxLayout();

    this->labelCrt->setText(QString::fromUtf8("  证书号 : "));
    this->labelCrt->setFixedWidth(50);
    this->labelShipName->setText(QString::fromUtf8("  船名 :    ") +this->shipInfo.shipName);
    this->comboBoxShipCrt->setFixedWidth(100);
    for(int i = 0;i<this->shipNumber;i++){
        this->comboBoxShipCrt->addItem(this->shipArray[i].crt);
    }

    this->labelTrim->setText(QString::fromUtf8("纵倾值 : "));
    this->labelTrim->setFixedWidth(50);
    this->doubleSpinBoxTrim->setMinimum(this->shipInfo.shipTrimMin);
    this->doubleSpinBoxTrim->setSingleStep(this->shipInfo.shipTrimStep);
    this->doubleSpinBoxTrim->setMaximum(this->shipInfo.shipTrimMin + this->shipInfo.shipTrimStep*this->shipInfo.capacityNumber);
    this->doubleSpinBoxTrim->setFixedWidth(60);
    this->doubleSpinBoxTrim->setValue(this->shipInfo.shipTrimMin);


    this->shipTrimMax = this->shipInfo.shipTrimMin +  this->shipInfo.shipTrimStep * (this->shipInfo.capacityNumber - 1) ;

    this->hBoxLayoutHeadInfo->addWidget(this->labelCrt);
    this->hBoxLayoutHeadInfo->addWidget(this->comboBoxShipCrt);
    this->hBoxLayoutHeadInfo->addWidget(this->labelShipName);

    this->hBoxLayoutHeadInfo->addWidget(this->labelTrim);
    this->hBoxLayoutHeadInfo->addWidget(this->doubleSpinBoxTrim);
//    this->hBoxLayoutHeadInfo->addWidget(this->labelFinalDate);

    this->widgetHeadInfo->setLayout(this->hBoxLayoutHeadInfo);
    connect(this->doubleSpinBoxTrim,SIGNAL(valueChanged(QString)),this,SLOT(shipTrimChanged(QString)) );
    connect(this->comboBoxShipCrt,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxShipCrtChanged(int )) );



}
void MainWindow::addWidgeFootInfo(void){
    this->pushButtonTotalCapacity = new QPushButton();
    this->pushButtonTotalCapacity->setText(QString::fromUtf8("   总容量  "));
    this->labelTotalCapacity = new QLabel();
    this->labelTotalCapacity->setText("      0.000");
    this->labelTotalCapacity->setFixedWidth(100);

    this->labelFinalDate =  new QLabel();
    this->labelFinalDate->setText(QString::fromUtf8("软件有效期 : ")+this->shipInfo.finalDate.toString("dd/MM/yyyy"));
    this->widgetFootInfo =  new QWidget();
    this->hBoxLayoutFootInfo = new QHBoxLayout();

    this->labelFinalDate->setFixedWidth(400);

    this->hBoxLayoutFootInfo->addWidget(this->labelFinalDate);
    this->hBoxLayoutFootInfo->addWidget(this->pushButtonTotalCapacity);
    this->hBoxLayoutFootInfo->addWidget(this->labelTotalCapacity);
    this->hBoxLayoutFootInfo->setAlignment(Qt::AlignRight);
    this->widgetFootInfo->setLayout(this->hBoxLayoutFootInfo);

    connect(this->pushButtonTotalCapacity,SIGNAL(clicked()),this,SLOT(pushButtonCalTotalCapacity()) );

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
        if((capacity_1 < 0) || capacity_2 < 0)
            return ;
        capacity = (capacity_2 - capacity_1 ) /10 * temp + capacity_1 ;
        qDebug()<<capacity <<capacity_1<<capacity_2;
    }
    else {
        capacity = this->queryTankCapacity(tankId,sounding);
        if(capacity< 0)
            return;
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
        QMessageBox::critical(NULL, QString::fromUtf8("数据库错误"), QString::fromUtf8("数据库错误,船只和舱室信息--> ")+QString("%1  %2").arg(shipId).arg(tankId), QMessageBox::Yes, QMessageBox::Yes);
        return -1;
    }
    //cal trim .add soon
//    float temp= (this->currentShipTrim -this->shipInfo.shipTrimMin );
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

void MainWindow::pushButtonCalTotalCapacity(void){
    qDebug()<<"cal total capacity";
    qDebug()<<this->geometry();
    float totalCapacity = 0;
    float eachCapacity = 0;
    Tank *tank;
    for(int i = 0;i<this->shipInfo.tankNumber;i++){
        tank = (Tank *)( this->widgetTankItems[i] );
        if (tank->checkDataValidator()){
            int tankId = tank->getTankId();
            int sounding = tank->getSounding();
            qDebug()<<"foreach-->"<<tankId<<sounding ;
            eachCapacity =  this->queryTankCapacity(tankId,sounding);
            if( eachCapacity >= 0){
                tank->setTankCapacity(eachCapacity);
                qDebug()<<"foreach-->"<<tankId<<sounding<<eachCapacity;
                totalCapacity += eachCapacity;
            }
            else {
                qDebug()<<"abort";
                return;
                break;

            }
        }
    }
    this->labelTotalCapacity->setText(QString("      %1").arg(totalCapacity));

}
void MainWindow::comboBoxShipCrtChanged(int index)
{
    qDebug()<<index <<this->shipArray[index].shipName << this->shipArray[index].crt;
    //update class info
    this->shipInfo =  this->shipArray[index];
    this->shipTrimMax = this->shipInfo.shipTrimMin + this->shipInfo.shipTrimStep  *(this->shipInfo.capacityNumber -1);
    //update head info
    this->labelShipName->setText(QString::fromUtf8("  船名 :    ") +this->shipInfo.shipName);

    this->doubleSpinBoxTrim->setMinimum(this->shipInfo.shipTrimMin);
    this->doubleSpinBoxTrim->setSingleStep(this->shipInfo.shipTrimStep);
    this->doubleSpinBoxTrim->setMaximum(this->shipTrimMax);
    this->doubleSpinBoxTrim->setValue(this->shipInfo.shipTrimMin);
    qDebug()<<this->doubleSpinBoxTrim->maximum()<<this->doubleSpinBoxTrim->minimum()<<this->doubleSpinBoxTrim->singleStep();
    //update tank items table
    this->update();
}

void MainWindow::addTankItemsTable(bool clearOld)
{
    if(clearOld){
        //do some thing to clear old
    }
}
