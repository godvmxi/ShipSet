#include "tank.h"
#include <QDebug>
#include <QRegExpValidator>


Tank::Tank(QWidget *parent) :
    QWidget(parent)
{
    this->labelName = new QLabel("");
//    this->labelName->setText(name);
    this->labelName->setFixedWidth(60);
    this->labelEqual = new QLabel("  -->  ");
    this->lineEditSounding = new QLineEdit();
    this->lineEditTemrature = new QLineEdit();
    this->lineEditTrim  = new QLineEdit();
    this->lineEditCapacity = new QLineEdit();


//    this->regExpValidatorTemperature  = new QRegExpValidator(QRegExp("^(-?\d+)(\.\d+)?$" ));
//    this->regExpValidatorSounding   = new QRegExpValidator(QRegExp("^\d+(\.\d+)?$"));
//    this->lineEditSounding->setValidator(this->regExpValidatorSounding);
//    this->lineEditTemrature->setValidator(this->regExpValidatorTemperature);

    this->pushButtonEqual = new QPushButton();
    this->pushButtonEqual->setText("  -->  ");

    this->mainLayout = new QHBoxLayout();

    this->mainLayout->addWidget(this->labelName);
    this->mainLayout->addWidget(this->lineEditSounding);
    this->mainLayout->addWidget(this->lineEditTemrature);

    this->mainLayout->addWidget(this->pushButtonEqual);
    this->mainLayout->addWidget(this->lineEditCapacity);



    this->setLayout(this->mainLayout);
    this->lineEditTrim->setEnabled(false);
    this->shipId = 0;
    connect(this->pushButtonEqual,SIGNAL(clicked()),this,SLOT(updateTankInfo()) );

    this->lineEditSounding->setText("4.200");
    this->lineEditTemrature->setText("20");
    this->lineEditCapacity->setText("0");
    this->lineEditCapacity->setEnabled(false);
}


Tank::~Tank()
{

}
float Tank::getCapicaity()
{
    return this->capacity;
}
void Tank::setTankTrim(float value){
    this->trim = value;
}
void Tank::setShipId( int id){
    this->shipId = id;
}
void Tank::setTankId( int id){
    this->tankId = id;
}
void Tank::setSoundingLimit(int min,int max){
    this->soundingMin = min;
    this->soundingMax = max;
    this->lineEditSounding->setText(QString("%1").arg((max +min )/20 * 10) );

    this->lineEditSounding->setToolTip( \
                QString("%1~~%2").arg(this->soundingMin).arg(this->soundingMax) );
}


void Tank::setTankName( QString name){
    this->tankName = name;
    this->labelName->setText(name);
}

void  Tank::setTankCapacity(TankInfo info)
{
    qDebug()<<info.shipId<<info.tankId<<info.sounding;
    return ;
}
bool Tank::setTankInfo(TankInfo info){
    if( (info.tankId != this->tankId) || ( info.shipId != this->shipId ) )
       return false;
   qDebug()<<"receive my tankInfo" <<this->shipId<<this->tankId;


   return true;
}
void Tank::updateTankInfo(void)
{
    if (this->checkDataValidator() ==  false)
        return ;

    qDebug()<<"try get capacity -> "<<this->shipId<<this->tankId<< this->sounding<<this->temprature;

    emit this->tryQueryBankInfo(this->tankId,this->sounding);
//    qDebug()<<"emit update tank info over";
    return ;
}
int Tank::getTankId(void){
   return this->tankId;
}
int Tank::getSounding(void){
    return this->sounding;
}

void Tank:: setTankCapacity(float value)
{
    double new_value = value *(1+2*0.0002*((double)this->temprature -20));
    this->capacity =  new_value ;
//    qDebug()<<"Tank-> "<<this->tankId <<"old capacity-> "<<value <<new_value;
    this->lineEditCapacity->setText(QString("%1").arg(new_value));
}
bool Tank::checkDataValidator(void){
    QString stringTemperature = this->lineEditTemrature->text();
    QString stringSounding =  this->lineEditSounding->text();
    if (stringTemperature.size() == 0 || stringSounding == 0){
//        qDebug()<<"temp or sounding can not be empty";
        this->setTankInvalid(true);
        return false;
    }

    bool ret = true;
    double temp = (stringSounding.toDouble(&ret) *1000);
    if((ret == false)){
        this->setTankInvalid(true);
        return false;
    };

    QString stringTemp = QString("%1").arg(temp);
//   这里把double转换成浮点，是避免浮点数转整数，精度丢失

    this->sounding =stringTemp.toInt(&ret,10)  ;
    if((this->sounding < this->soundingMin || this->sounding > this->soundingMax ) && (ret == false)){

        this->setTankInvalid(true);
        return false;
    }
    else {
        this->setTankInvalid(false);
    }

    qDebug()<<stringSounding << temp<<stringTemp <<this->sounding;

    qDebug()<<"sounding --> "<<stringSounding  << this->sounding;

    this->temprature = stringTemperature.toFloat();

    this->setTankInvalid(false);
    return true;
}
void Tank::setTankInvalid(bool enable)
{
//    QPalette pal(palette());

    if(enable){
//        pal.setBrush(QPalette::Base, Qt::red);
        this->lineEditSounding->setStyleSheet("background-color:red;");
        this->lineEditTemrature->setStyleSheet("background-color:red;");
        this->lineEditCapacity->setStyleSheet("background-color:red;");

        this->lineEditCapacity->setText("");
    }
    else {
        this->lineEditSounding->setStyleSheet("background-color:white;");
        this->lineEditTemrature->setStyleSheet("background-color:white;");
        this->lineEditCapacity->setStyleSheet("background-color:white;");
    }
//    this->setAutoFillBackground(true);
//    setPalette(pal);

//    this->update();
}
QString Tank::getTankName(void){
    return this->labelName->text();
}
