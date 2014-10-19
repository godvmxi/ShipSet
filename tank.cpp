#include "tank.h"


tank::tank(QWidget *parent,QString name) :
    QWidget(parent)
{
    this->labelName = new QLabel("");
    this->labelName->setText(name);
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

tank::~tank()
{

}
float tank::getCapicaity()
{
    return this->capacity;
}
void tank::setTankTrim(float value){
    this->trim = value;
}
int tank::setShipId( int id){
    this->shipId = id;
}
int tank::setTankCapacity(TankInfo info)
{
    return true;
}
