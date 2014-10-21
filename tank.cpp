#include "tank.h"


Tank::Tank(QWidget *parent) :
    QWidget(parent)
{
    this->labelName = new QLabel("");
//    this->labelName->setText(name);
    this->labelName->setFixedWidth(80);
    this->labelEqual = new QLabel("  -->  ");
    this->lineEditSounding = new QLineEdit();
    this->lineEditTemrature = new QLineEdit();
    this->lineEditTrim  = new QLineEdit();
    this->lineEditCapacity = new QLineEdit();

    this->mainLayout = new QHBoxLayout();

    this->mainLayout->addWidget(this->labelName);
    this->mainLayout->addWidget(this->lineEditSounding);
    this->mainLayout->addWidget(this->lineEditTemrature);
    this->mainLayout->addWidget(this->lineEditTrim);
    this->mainLayout->addWidget(this->labelEqual);
    this->mainLayout->addWidget(this->lineEditCapacity);

    this->setLayout(this->mainLayout);
    this->lineEditTrim->setEnabled(false);
    this->shipId = 0;
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
    return ;
}
