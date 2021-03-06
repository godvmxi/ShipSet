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

    this->shipInfo = this->shipArray[0];


    showShipInfo(&this->shipInfo);
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

    this->addTankItemsTable(false);

    this->widgetTankItemsTable->setLayout(this->vBoxLayoutTankItemsTable);
    this->scrollAreaTankItemsTable->setWidget(this->widgetTankItemsTable);
    this->scrollAreaTankItemsTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //this->mainLayout->addWidget(this->scrollAreaTankItemsTable);
    this->widgetMiddleMain = new QWidget();
    this->widgetMiddleRight = new QWidget();
    this->hBoxLayoutMiddleMain = new QHBoxLayout();
    this->formLayoutMiddleRight = new QFormLayout();

//    this->widgetMiddleRight->setFixedWidth(00);
    this->spinBoxTrimH  = new QDoubleSpinBox();
    this->spinBoxTrimV = new QDoubleSpinBox();
    this->lineEditOil  = new QLineEdit();
    this->lineEditDensity  = new QLineEdit();
    this->lineEditVolume  = new QLineEdit();
    this->lineEditTotalCapity = new QLineEdit();
    this->lineEditTotalCapity->setReadOnly(true);
    this->lineEditTotalCapity->setText("0.0");
    //init spinBox TrimH
    qDebug()<<"Trim H->"<<this->shipInfo.shipTrimH[0] << this->shipInfo.shipTrimH[this->shipInfo.capacityNumber-1];
    this->spinBoxTrimH->setMinimum(this->shipInfo.shipTrimH[0]);
    this->spinBoxTrimH->setMaximum(this->shipInfo.shipTrimH[this->shipInfo.capacityNumber-1]);
    this->spinBoxTrimH->setSingleStep(0.05);
    this->spinBoxTrimH->setValue((this->spinBoxTrimH->maximum() + this->spinBoxTrimH->minimum() ) /2 );
    qDebug()<<"Trim V->"<<this->shipInfo.shipTrimV[0] << this->shipInfo.shipTrimV[this->shipInfo.capacityNumber-1];
    this->spinBoxTrimV->setMinimum(this->shipInfo.shipTrimV[0]);
    this->spinBoxTrimV->setMaximum(this->shipInfo.shipTrimV[this->shipInfo.capacityNumber-1]);
    this->spinBoxTrimV->setSingleStep(0.05);
    this->spinBoxTrimV->setValue((this->spinBoxTrimV->maximum() + this->spinBoxTrimV->minimum() ) /2 );



    this->formLayoutMiddleRight->addRow(QString::fromUtf8("横    倾"),this->spinBoxTrimH);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("纵    倾"),this->spinBoxTrimV);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("管内油量"),this->lineEditOil);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("标准密度"),this->lineEditDensity);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("体积修正"),this->lineEditVolume);
    this->formLayoutMiddleRight->addRow(QString::fromUtf8("总 容 积"),this->lineEditTotalCapity);

    this->lineEditOil->setText("0");
    this->lineEditDensity->setText("0");
    this->lineEditVolume->setText("0");
    this->lineEditVolume->setText("0");

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
    connect(this->pushButtonTotalCapacity,SIGNAL(clicked()),this,SLOT(pushButtonCalTotalCapacity()) ) ;
    updateWidgetsToolTips();

    this->feedback  = new QFeedBack(this,FEEDBACK_SERVER_URL);
    this->feedback->report("starting");

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
    float shipTrimFixV = 0;
    float shipTrimFixH = 0;
    TankInfo tankInfo = {0};
    tankInfo.shipId =  this->shipInfo.shipId;
    tankInfo.tankId =  tankId;
    tankInfo.sounding = sounding;


    this->shipTrimH = this->spinBoxTrimH->value();
    this->shipTrimV = this->spinBoxTrimV->value();

    if( ! calTankFixCapacityValue(&tankInfo,&capacity) ){
        //deal error
        return ;
    }
}
bool  MainWindow::queryTankCapacity(int tankId,int sounding,float *cap)
{
    qDebug()<<__func__ ;
    TankInfo resultInfo;
    if ( this->sqlCore->queryTankInfo(this->shipInfo.shipId,tankId,sounding,&resultInfo ) ){
        qDebug()<<"find capacity";
        return true;
    }
    qDebug()<<"can find directly ,try more ";
    this->showSoundingQueryError(tankId);
    return false ;

}
void MainWindow::shipTrimChanged(QString d){

    float trim = d.toFloat();
    if((trim < this->shipInfo.shipTrimMin) || (trim >this->shipTrimMax )){
        this->doubleSpinBoxTrim->setValue(this->currentShipTrim);
        return ;
    }
    this->currentShipTrim = trim;
}


void MainWindow::pushButtonCalTotalCapacity(void){
    qDebug()<<"cal total capacity";
    //qDebug()<<this->geometry();
    float totalCapacity = 0;
    float eachCapacity = 0;

    TankInfo tankInfo = {0} ;


    this->shipTrimH = this->spinBoxTrimH->value();
    this->shipTrimV = this->spinBoxTrimV->value();


    Tank *tank;
    for(int i = 0;i<this->shipInfo.tankNumber;i++){

        tank = (Tank *)( this->widgetTankItems[i] );

        if (tank->checkDataValidator()){
            initTankInfo(&tankInfo);
            tankInfo.shipId = this->shipInfo.shipId;
            tankInfo.tankId = tank->getTankId();
            tankInfo.sounding = tank->getSounding();

            if( ! calTankFixCapacityValue(&tankInfo,&eachCapacity) ){
                //deal error
                return ;
            }
            qDebug()<<"foreach-->"<<tankInfo.tankId<<tankInfo.sounding << eachCapacity;
            if(eachCapacity >= 0){
//                tank->setTankCapacity(eachCapacity);
            }
            else {
                qDebug()<<"cal total value error ,abort";
                return;
            }
        }
        else {
            eachCapacity = 0;
            qDebug()<<"skip no data tank -->"<< tank->getTankId();;
        }
         totalCapacity += eachCapacity;

    }
    qDebug()<<"total capacity -> "<<totalCapacity ;
    this->lineEditTotalCapity->setText(QString("%1").arg(totalCapacity));

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
        itemTank->setSoundingLimit(this->shipInfo.soundingLimit[0],this->shipInfo.soundingLimit[1]);
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
   // qDebug()<<"tank error show -> "<<tankId;
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
    this->spinBoxTrimH->setToolTip(
                QString("%1~~%2").arg(this->shipInfo.shipTrimH[0])
                .arg(this->shipInfo.shipTrimH[this->shipInfo.capacityNumber-1]));
    this->spinBoxTrimV->setToolTip(
                QString("%1~~%2").arg(this->shipInfo.shipTrimV[0])
                .arg(this->shipInfo.shipTrimV[this->shipInfo.capacityNumber-1]));

    //update all tank tool tips
}


bool MainWindow::calTankFixCapacityValue(TankInfo *info ,float *retValue){
    float capacity = 0;
    float shipTrimFixV = 0;
    float shipTrimFixH = 0;
    int sounding = info->sounding ;
    int tankId = info->tankId;
    int shipId = info->shipId ;
    int trimType = info->soundingType;
//    TankInfo tankInfo = {0};


    TankInfo infoV[2] = {0};
    TankInfo infoH[2] = {0};
    TankInfo infoC[2] = {0};
    infoV[0] =  *info;
    infoV[0].soundingType =  TANK_TRIM_V_VALUE ;
    infoH[0] =  *info;
    infoH[0].soundingType =  TANK_TRIM_H_VALUE ;


    if( !this->sqlCore->queryTankValueArray(infoV) ){

        return false;
    }

    if( !this->sqlCore->queryTankValueArray(infoH) ){
        return false;
    }

    //cal accruate capicity
//    showTankInfo(infoV);
//    showTankInfo(infoV+1);
//    showTankInfo(infoH);
//    showTankInfo(infoH+1);



    if(! calTankFixValueFromInfo(infoV,info->sounding) ){

        return  false;
    }
//    showTankInfo(infoV) ;

    if(! calTankFixValueFromInfo(infoH ,info->sounding) ){

           return  false;
    }

//    showTankInfo(infoH);

    calTankFixValueByTrim(infoV,&shipTrimFixV);
    calTankFixValueByTrim(infoH,&shipTrimFixH);
    qDebug()<<"fix trim H -> "<< shipTrimFixH;
    qDebug()<<"fix trim V -> "<< shipTrimFixV;

    int fixedSounding =  sounding + shipTrimFixH + shipTrimFixV;
    infoC[0] =  *info;
    infoC[0].soundingType =  TANK_CAPICITY_VALUE ;
    infoC[0].sounding =  fixedSounding;
    qDebug()<<"fix sounding  -> "<< sounding <<fixedSounding ;
    if( !this->sqlCore->queryTankValueArray(infoC) ){

        return false;
    }
    if(! calTankFixValueFromInfo(infoC,fixedSounding) ){

        return  false;
    }
    showTankInfo(infoC);
    float result = infoC->capacity[0] + infoC->capacity[1]*3/10 ;

    Tank *tank =  (Tank *)this->widgetTankItems[info->tankId - 1];\

    tank->setTankCapacity(result);
    *retValue =  tank->getCapicaity();
    qDebug()<<"tank capacity ->" <<result << *retValue;
    return true;


}

bool MainWindow::calTankFixValueFromInfo(TankInfo *info ,int sounding){
    if(!convertStringValueToList(info[0].capacity,info[0].strValue,this->shipInfo.capacityNumber) ){
        return false;
    }
    if(!convertStringValueToList(info[1].capacity,info[1].strValue,this->shipInfo.capacityNumber) ){
        return false;
    }
//    qDebug()<<"info 0";
//    showTankInfo(info);
//    qDebug()<<"info 1";
//    qDebug()<<"origin sounding -> "<<info[0].sounding;
//    showTankInfo(info+1);
    if ( info[0].sounding ==  info[1].sounding ){
        //do simple cal
        qDebug()<<"do simple cal";

        //just call trim

    }
    else {
        //do comlex cal,first call capcity
        qDebug()<<"do complex cal";

        for (int i = 0 ;i <this->shipInfo.capacityNumber;i++){
            info[0].capacity[i] = info[0].capacity[i] + ((sounding-info[0].sounding) *(info[1].capacity[i] - info[0].capacity[i]) /(info[1].sounding-info[0].sounding)  );
        }
        info[0].sounding =  sounding;

    }
    qDebug()<<"info ok -> " << sounding << info[0].sounding ;
    showTankInfo(info);

}

bool MainWindow::calTankFixValueByTrim(TankInfo *info ,float *retValue){
    int trimType = info->soundingType;
    float *trimValue = NULL;
    float shipTrim ;
    if (trimType == TANK_TRIM_H_VALUE) {
        trimValue = this->shipInfo.shipTrimH;
        shipTrim = this->shipTrimH;
    }
    else  if (trimType == TANK_TRIM_V_VALUE) {
        trimValue = this->shipInfo.shipTrimV;
        shipTrim = this->shipTrimV;
    }
    else {
        trimValue = NULL;
        return false;
    }
    for (int i = 1 ;i< this->shipInfo.capacityNumber;i++){
        if (shipTrim == trimValue[i-1]){
            *retValue = info[0].capacity[i-1];
            break;
        }
        else if (shipTrim == trimValue[i]){
            *retValue = info[0].capacity[i];
            break;
        }
        else if(  (shipTrim > trimValue[i-1]  )&& (shipTrim < trimValue[i])) {
            *retValue = trimValue[i-1] + (shipTrim -  trimValue[i-1]) *(info->capacity[i]- info->capacity[i-1]) /(trimValue[i]-trimValue[i-1]) ;
            break;
        }
    }
    qDebug()<<"capacity -> " <<*retValue ;
    return true;


}
