#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tank.h"
#include <stdio.h>
#include <string.h>
#include <QDebug>
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->mainLayout =  new QVBoxLayout();
    this->setFixedWidth(480);
//    this->setWindowIcon(QIcon(":/icon.bmp"));

    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("ships.db");
    this->shipNumber = this->sqlCore->queryShipNumber();
    if((this->shipNumber <= 0 )||(this->shipNumber > MAX_SHIP_NUMBER)){
        QMessageBox::critical(NULL, QString::fromUtf8("船只数据库损坏"), QString::fromUtf8("船只数据库损坏-->")+QString("%1 ").arg(this->shipNumber), QMessageBox::Yes, QMessageBox::Yes);
        exit(0);
        return ;
    }
    this->sqlCore->queryShipsInfo(this->shipArray);

//    for(int i = 0 ;i<this->shipNumber;i++){
//        qDebug()<<"shipName --> "<<this->shipArray[i].shipName;
//    }
    this->shipInfo = this->shipArray[0];


    this->addWidgeHeadInfo();
    this->mainLayout->addWidget(this->widgetHeadInfo);
    this->labelTableTitle = new QLabel();
#ifdef Q_OS_WIN32
    //modify here ,xuelong for windows
    this->labelTableTitle->setText(QString::fromUtf8("   舱名       测深高度       温度           计算         容量值"));
  #else
    this->labelTableTitle->setText(QString::fromUtf8("      舱名              测深高度            温度                    计算                    容量值"));
#endif

    this->mainLayout->addWidget(this->labelTableTitle);

    this->widgetTankItemsTable = new QWidget();
    this->widgetTankItemsTable->setFixedWidth(440);
    this->vBoxLayoutTankItemsTable = new QVBoxLayout();
    this->scrollAreaTankItemsTable = new QScrollArea();
    this->scrollAreaTankItemsTable->setFixedWidth(460);
#if 0
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
#else
    this->addTankItemsTable(false);
#endif
    this->widgetTankItemsTable->setLayout(this->vBoxLayoutTankItemsTable);
    this->scrollAreaTankItemsTable->setWidget(this->widgetTankItemsTable);
    this->scrollAreaTankItemsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mainLayout->addWidget(this->scrollAreaTankItemsTable);

    this->addWidgeFootInfo();
    this->mainLayout->addWidget(this->widgetFootInfo);
    ui->centralWidget->setLayout(this->mainLayout);
    this->setFixedHeight(this->getWindowsHeight());

    //set backgroud
    this->pixmapBackgroud = new QPixmap();
    qDebug()<<this->pixmapBackgroud->load(":/res/res/bk0.jpg");
    qDebug()<<"bk ipeg --> "<<this->pixmapBackgroud->isNull();
//    this->setAutoFillBackground(true);
    QPalette    palette = this->palette();
    palette.setBrush(this->backgroundRole(),
                     QBrush(this->pixmapBackgroud->scaled(this->size(),
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation)));
    this->setPalette(palette);

//   this->scrollAreaTankItemsTable->setStyleSheet("border:1px; background-color:transparent ");
//    this->scrollAreaTankItemsTable->

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addWidgeHeadInfo(void){


    this->labelCrt = new QLabel();
    this->labelShipName = new QLabel();
    this->comboBoxShipCrt =  new QComboBox();

//    this->comboBoxShipCrt->setStyleSheet("border:1px; background-color:transparent ");
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
    this->doubleSpinBoxTrim->setSingleStep(double(this->shipInfo.shipTrimStep));
    this->doubleSpinBoxTrim->setDecimals(4);
    this->doubleSpinBoxTrim->setMaximum(this->shipInfo.shipTrimMin + this->shipInfo.shipTrimStep*this->shipInfo.capacityNumber);
    this->doubleSpinBoxTrim->setFixedWidth(60);
    this->doubleSpinBoxTrim->setValue(this->shipInfo.shipTrimMin);


    this->shipTrimMax = this->shipInfo.shipTrimMin +  this->shipInfo.shipTrimStep * (this->shipInfo.capacityNumber - 1) ;
    this->currentShipTrim = this->doubleSpinBoxTrim->value();

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
    this->pushButtonAbout = new QPushButton(QString::fromUtf8("更多"));
    this->labelTotalCapacity->setText("      0.000");
    this->labelTotalCapacity->setFixedWidth(100);

    this->labelFinalDate =  new QLabel();
    this->labelFinalDate->setText(QString::fromUtf8("软件有效期 : ")+this->shipInfo.finalDate.toString("dd/MM/yyyy"));
    this->widgetFootInfo =  new QWidget();
    this->hBoxLayoutFootInfo = new QHBoxLayout();

    this->labelFinalDate->setFixedWidth(400);

    this->hBoxLayoutFootInfo->addWidget(this->labelFinalDate);
    this->hBoxLayoutFootInfo->addWidget(this->pushButtonAbout);
    this->hBoxLayoutFootInfo->addWidget(this->pushButtonTotalCapacity);
    this->hBoxLayoutFootInfo->addWidget(this->labelTotalCapacity);
    this->hBoxLayoutFootInfo->setAlignment(Qt::AlignRight);
    this->widgetFootInfo->setLayout(this->hBoxLayoutFootInfo);




    connect(this->pushButtonTotalCapacity,SIGNAL(clicked()),this,SLOT(pushButtonCalTotalCapacity()) );
    connect(this->pushButtonAbout,SIGNAL(clicked()),this,SLOT(pushButtonAboutSlot()) );
}

void MainWindow::updateWidgetTankTrim(void){

}

void MainWindow::queryTankInfoSlot(int tankId,int sounding)
{
    float capacity = 0;
//    if(sounding%10 != 0){
//        int temp = sounding %10 ;
//        int sounding_1 =  sounding -  temp;
//        int sounding_2 =  sounding_1 +10;
//        qDebug()<<"will query two sounding -> "<<tankId << sounding_1 << sounding_2;
//        float capacity_1 = this->queryTankCapacity(tankId,sounding_1);
//        float capacity_2 = this->queryTankCapacity(tankId,sounding_2);
//        if((capacity_1 < 0) || capacity_2 < 0)
//            return ;
//        capacity = (capacity_2 - capacity_1 ) /10 * temp + capacity_1 ;
//        qDebug()<<capacity <<capacity_1<<capacity_2;
//    }
//    else {
//        capacity = this->queryTankCapacity(tankId,sounding);
//        if(capacity< 0)
//            return;
//    }
    capacity = queryTankCapacity(tankId,sounding);
    Tank *tank =  (Tank *)this->widgetTankItems[tankId - 1];
    //temprature modify
    //add trim fix
    //

    tank->setTankCapacity(capacity);


}
float  MainWindow::queryTankCapacity(int tankId,int sounding)
{
    float capacity = 0;

    TankInfo resultInfo;
    if(sounding%10 != 0){
        int temp = sounding %10 ;
        int sounding_1 =  sounding -  temp;
        int sounding_2 =  sounding_1 +10;
        qDebug()<<"will query two sounding -> "<<tankId << sounding_1 << sounding_2;

        resultInfo        = this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding_1);
        TankInfo tempInfo = this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding_2);


        qDebug()<<resultInfo.sounding<<resultInfo.capacity[0]<<resultInfo.capacity[1]<<resultInfo.capacity[2]\
                  <<resultInfo.capacity[3]<<resultInfo.capacity[4]<<resultInfo.capacity[5]\
                    <<resultInfo.capacity[6]<<resultInfo.capacity[7];
        qDebug()<<tempInfo.sounding<<tempInfo.capacity[0]<<tempInfo.capacity[1]<<tempInfo.capacity[2]\
                  <<tempInfo.capacity[3]<<tempInfo.capacity[4]<<tempInfo.capacity[5]\
                    <<tempInfo.capacity[6]<<tempInfo.capacity[7];

        for(int i = 0 ;i <this->shipInfo.capacityNumber;i++){
            resultInfo.capacity[i] += (tempInfo.capacity[i] - resultInfo.capacity[i] ) /10 * temp ;
        }
        resultInfo.sounding = sounding;

    }
    else {
        qDebug()<<"will query one sounding -> "<<tankId << sounding  ;
        resultInfo        = this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding);
    }
    qDebug()<<resultInfo.sounding<<resultInfo.capacity[0]<<resultInfo.capacity[1]<<resultInfo.capacity[2]\
              <<resultInfo.capacity[3]<<resultInfo.capacity[4]<<resultInfo.capacity[5]\
                <<resultInfo.capacity[6]<<resultInfo.capacity[7];

    //cal trim .add soon
//    float temp= (this->currentShipTrim -this->shipInfo.shipTrimMin );
    int trimFlag = -1; // 0:正好整除, 1:介于二者之间,具体查看min ~max
    float min = 0;
    float max;
    int i =0;
    min = this->shipInfo.shipTrimMin;
    max = min + this->shipInfo.shipTrimStep ;
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
        qDebug()<<"just ok  -> "<<i<<min<< max;
        return resultInfo.capacity[i];
    }
    else if(trimFlag == 1){
        qDebug()<<"not just ok ->  "<< this->currentShipTrim <<min << max;
        float capacity_1 =  resultInfo.capacity[i];
        float capacity_2 =  resultInfo.capacity[i+1];
        return    ( (capacity_2 -capacity_1 ) /this->shipInfo.shipTrimStep )*(this->currentShipTrim -min)  + capacity_1 ;
    }
    else
        return resultInfo.capacity[i];
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
    //qDebug()<<"cal total capacity";
    //qDebug()<<this->geometry();
    float totalCapacity = 0;
    float eachCapacity = 0;
    Tank *tank;
    for(int i = 0;i<this->shipInfo.tankNumber;i++){
        tank = (Tank *)( this->widgetTankItems[i] );
        if (tank->checkDataValidator()){
            int tankId = tank->getTankId();
            int sounding = tank->getSounding();
           // qDebug()<<"foreach-->"<<tankId<<sounding ;
            eachCapacity =  this->queryTankCapacity(tankId,sounding);
            //     qDebug()<<"foreach-->"<<tankId<<sounding<<eachCapacity;
            if(eachCapacity >= 0){
                tank->setTankCapacity(eachCapacity);
            }
            else {
                qDebug()<<"cal total value error ,abort";
                return;
                break;
            }
        }
        else {
            eachCapacity = 0;
            qDebug()<<"skip no data tank -->"<< tank->getTankId();;
        }
         totalCapacity += eachCapacity;

    }
    this->labelTotalCapacity->setText(QString("      %1").arg(totalCapacity));

}
void MainWindow::comboBoxShipCrtChanged(int index)
{
    qDebug()<<index <<this->shipArray[index].shipName << this->shipArray[index].crt;
    //update class info
    this->oldShipInfo =  this->shipInfo;
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
    addTankItemsTable(true);
    this->widgetTankItemsTable->setFixedHeight(60*this->shipInfo.tankNumber);
    this->update();
    qDebug()<<"table item number->"<<this->vBoxLayoutTankItemsTable->count();
    this->setFixedHeight(this->getWindowsHeight());
}

void MainWindow::addTankItemsTable(bool clearOld)
{
//    qDebug()<<"update tank Item table ->" <<this->shipInfo.shipName<<this->shipInfo.tankNumber;
    if(clearOld){
        //do some thing to clear old
        qDebug()<<"will remove item number -> "<<this->oldShipInfo.tankNumber;
        Tank *tank;
        for (int i  = 0;i<this->oldShipInfo.tankNumber;i++){
            tank = (Tank *)this->widgetTankItems[i];
            this->vBoxLayoutTankItemsTable->removeWidget(tank);
            delete tank;
            this->widgetTankItems[i] = 0;
        }
    }
//    qDebug()<<"will add item number -> "<<this->shipInfo.tankNumber;
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
}

int MainWindow::getWindowsHeight(void){
    int result = 0;
    int base = 170;
    if(this->shipInfo.tankNumber > 8)
        result =  base + (8 *60 );
    else
        result =  base + (this->shipInfo.tankNumber *60 );
//    qDebug()<<"window height --> "<<result;
    return result;
}
void MainWindow::pushButtonAboutSlot(void)
{
    DialogAbout about;
    about.exec();
}
