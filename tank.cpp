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
//    this->mainLayout->addWidget(this->lineEditTrim);
//    this->mainLayout->addWidget(this->labelEqual);
    this->mainLayout->addWidget(this->pushButtonEqual);
    this->mainLayout->addWidget(this->lineEditCapacity);



    this->setLayout(this->mainLayout);
    this->lineEditTrim->setEnabled(false);
    this->shipId = 0;
    connect(this->pushButtonEqual,SIGNAL(clicked()),this,SLOT(updateTankInfo()) );

    this->lineEditSounding->setText("4.200");
    this->lineEditTemrature->setText("20");
    this->lineEditCapacity->setText("0");
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
        qDebug()<<"temp or sounding can not be empty";
        return false;
    }
    this->temprature = stringTemperature.toFloat();
    this->sounding =int  (stringSounding.toDouble() * 1000);
    return true;
}
