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
//    this->setFixedWidth(480);
//    this->setWindowIcon(QIcon(":/icon.bmp"));

    this->shipId = 0;
    this->sqlCore =  new sqlUtils();
    this->sqlCore->setDbFile("Qt5Script.dll");
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
    this->labelTableTitle->setText(QString::fromUtf8("     舱名       测深高度       温度           计算         容量值                公共参数设定"));
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

    //this->mainLayout->addWidget(this->scrollAreaTankItemsTable);
    this->widgetMiddleMain = new QWidget();
    this->widgetMiddleRight = new QWidget();
    this->hBoxLayoutMiddleMain = new QHBoxLayout();
    this->formLayoutMiddleRight = new QFormLayout();

//    this->widgetMiddleRight->setFixedWidth(00);
    this->lineEditTrimH  = new QLineEdit();
    this->lineEditTrimV = new QLineEdit();
    this->lineEditOil  = new QLineEdit();
    this->lineEditDensity  = new QLineEdit();
    this->lineEditVolume  = new QLineEdit();
    this->lineEditTotalCapity = new QLineEdit();
    this->lineEditTotalCapity->setReadOnly(true);
    this->lineEditTotalCapity->setText("0.0");

    this->formLayoutMiddleRight->addRow(QString::fromUtf8("横    倾"),this->lineEditTrimH);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("纵    倾"),this->lineEditTrimV);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("管内油量"),this->lineEditOil);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("标准密度"),this->lineEditDensity);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("体积修正"),this->lineEditVolume);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("总 容 积"),this->lineEditTotalCapity);

    this->formLayoutMiddleRight->setVerticalSpacing(25);
    this->formLayoutMiddleRight->setHorizontalSpacing(10);
    this->pushButtonTotalCapacity = new QPushButton();
    this->pushButtonTotalCapacity->setText(QString::fromUtf8("   总容量  "));
    this->formLayoutMiddleRight->addRow(this->pushButtonTotalCapacity);



    this->widgetMiddleRight->setLayout(this->formLayoutMiddleRight);

    this->hBoxLayoutMiddleMain->addWidget(this->scrollAreaTankItemsTable);
    this->hBoxLayoutMiddleMain->addWidget(this->widgetMiddleRight);
    this->widgetMiddleMain->setLayout(this->hBoxLayoutMiddleMain);



    this->mainLayout->addWidget((this->widgetMiddleMain));



//    this->mainLayout->addWidget(this->widgetFootInfo);
    ui->centralWidget->setLayout(this->mainLayout);
    this->setFixedHeight(this->getWindowsHeight());

    //set backgroud
    this->pixmapBackgroud = new QPixmap();
    this->pixmapBackgroud->load(":/res/res/bk0.jpg");
    if(this->pixmapBackgroud->isNull()){
        QMessageBox::critical(NULL, QString::fromUtf8("ERROR"), QString::fromUtf8("程序资源丢失1"), QMessageBox::Yes, QMessageBox::Yes);
        exit(0);
    }
//    this->setAutoFillBackground(true);
    QPalette    palette = this->palette();
    palette.setBrush(this->backgroundRole(),
                     QBrush(this->pixmapBackgroud->scaled(this->size(),
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation)));
//    this->centralWidget()->setPalette(palette);
    this->setPalette(palette);

//   this->scrollAreaTankItemsTable->setStyleSheet("border:1px; background-color:transparent ");
//    this->scrollAreaTankItemsTable->

    this->labelError = new QLabel();
    this->statusBar()->addWidget(this->labelError);

    updateWidgetsToolTips();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addWidgeHeadInfo(void){


    this->labelCrtName = new QLabel();
    this->labelShipName = new QLabel();
    this->comboBoxShipCrtName =  new QComboBox();

//    this->comboBoxShipCrtName->setStyleSheet("border:1px; background-color:transparent ");
    this->labelTrim = new QLabel();
    this->doubleSpinBoxTrim = new QDoubleSpinBox();

    this->widgetHeadInfo = new QWidget();
    this->hBoxLayoutHeadInfo = new QHBoxLayout();

    this->labelCrtName->setText(QString::fromUtf8("  证书号 : "));
    this->labelCrtName->setFixedWidth(50);
    this->labelShipName->setText(QString::fromUtf8("  船名 :    ") +this->shipInfo.shipName);
    this->labelShipName->setFixedWidth(150);
    this->comboBoxShipCrtName->setFixedWidth(100);
    for(int i = 0;i<this->shipNumber;i++){
        this->comboBoxShipCrtName->addItem(this->shipArray[i].crtName);
    }


    this->hBoxLayoutHeadInfo->addWidget(this->labelCrtName);
    this->hBoxLayoutHeadInfo->addWidget(this->comboBoxShipCrtName);
    this->hBoxLayoutHeadInfo->addWidget(this->labelShipName);

    this->labelTotalCapacity = new QLabel();
//    this->pushButtonAbout = new QPushButton(QString::fromUtf8("更多"));
//    this->labelTotalCapacity->setText("      0.000");
//    this->labelTotalCapacity->setFixedWidth(100);

    this->labelFinalDate =  new QLabel();
    this->labelFinalDate->setText(QString::fromUtf8("软件有效期 : ")+this->shipInfo.crtValidDate.toString("dd/MM/yyyy"));

    this->labelFinalDate->setFixedWidth(400);
    this->hBoxLayoutHeadInfo->addWidget(this->labelFinalDate);

//    this->hBoxLayoutHeadInfo->addWidget(this->labelTrim);
//    this->hBoxLayoutHeadInfo->addWidget(this->doubleSpinBoxTrim);
//    this->hBoxLayoutHeadInfo->addWidget(this->labelFinalDate);

    this->widgetHeadInfo->setLayout(this->hBoxLayoutHeadInfo);
    connect(this->doubleSpinBoxTrim,SIGNAL(valueChanged(QString)),this,SLOT(shipTrimChanged(QString)) );
    connect(this->comboBoxShipCrtName,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxShipCrtNameChanged(int )) );



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
    if( !queryTankCapacity(tankId,sounding,&capacity) )
    {
        this->showSoundingQueryError(tankId);
        return ;
    }

    Tank *tank =  (Tank *)this->widgetTankItems[tankId - 1];
    //temprature modify
    //add trim fix
    //

    tank->setTankCapacity(capacity);


}
bool  MainWindow::queryTankCapacity(int tankId,int sounding,float *cap)
{

    TankInfo resultInfo;
    if(sounding%10 != 0){
        int temp = sounding %10 ;
        int sounding_1 =  sounding -  temp;
        int sounding_2 =  sounding_1 +10;
        TankInfo tempInfo ;
        memset(&tempInfo,0,sizeof(TankInfo));
        qDebug()<<"will query two sounding -> "<<tankId << sounding_1 << sounding_2;

        if ( !this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding_1,&resultInfo ) ){
            qDebug()<<"tank exceed the max";
            this->showSoundingQueryError(tankId);
            return false ;
        }
        if( !this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding_2,&tempInfo) )
        {
            qDebug()<<"tank exceed the max";
            this->showSoundingQueryError(tankId);
            return false ;
        }


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
        if(!this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding,&resultInfo) )
        {
                qDebug()<<"tank exceed the max";
                return false ;
        }
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
//        qDebug()<<"just ok  -> "<<i<<min<< max;
        *cap = resultInfo.capacity[i];
    }
    else if(trimFlag == 1){
        qDebug()<<"not just ok ->  "<< this->currentShipTrim <<min << max;
        float capacity_1 =  resultInfo.capacity[i];
        float capacity_2 =  resultInfo.capacity[i+1];
        *cap =    ( (capacity_2 -capacity_1 ) /this->shipInfo.shipTrimStep )*(this->currentShipTrim -min)  + capacity_1 ;

    }
    else
        *cap = resultInfo.capacity[i];
    this->labelError->setText("");
    return true;
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
            if(!this->queryTankCapacity(tankId,sounding,&eachCapacity))
            {
                qDebug()<<"show sounding error";
                this->showSoundingQueryError(tankId);
                return ;
            }

                 qDebug()<<"foreach-->"<<tankId<<sounding<<eachCapacity;
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
void MainWindow::comboBoxShipCrtNameChanged(int index)
{
    qDebug()<<index <<this->shipArray[index].shipName << this->shipArray[index].crtName;
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
void MainWindow::showSoundingQueryError(int tankId){
    if(tankId < 0){
        return ;
    }
    Tank *tank;
    qDebug()<<"tank error show -> "<<tankId;
    for(int i = 0;i<this->shipInfo.tankNumber;i++){
        tank = (Tank *)( this->widgetTankItems[i] );
        if(tankId == tank->getTankId()){
            tank->setTankInvalid(true);
            this->labelError->setText(tank->getTankName() +QString::fromUtf8("  测深高度值超出最大值，请输入正确值"));
            break;
        }
    }

//    QMessageBox::information(NULL, "Error",tank->getTankName()+ QString::fromUtf8("  测深高度值超出最大值，请输入正确值"), QMessageBox::Yes , QMessageBox::Yes);

}
void MainWindow::updateWidgetsToolTips(void){
    this->lineEditTrimH->setToolTip(
                QString("%1~~%2").arg(this->shipInfo.shipTrimH[0])
                .arg(this->shipInfo.shipTrimH[this->shipInfo.capacityNumber-1]));
    this->lineEditTrimV->setToolTip(
                QString("%1~~%2").arg(this->shipInfo.shipTrimV[0])
                .arg(this->shipInfo.shipTrimV[this->shipInfo.capacityNumber-1]));

    //update all tank tool tips
}
