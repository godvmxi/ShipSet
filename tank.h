#ifndef TANK_H
#define TANK_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QString>
#include "common.h"


namespace Ui {
class Tank;
}

class Tank : public QWidget
{
    Q_OBJECT

public:
    explicit Tank(QWidget *parent = 0,QString name =tr("NULL"));
    ~Tank();
    float getCapicaity();
    void setTankTrim(float value);
    int setShipId(int id);
    int setTankCapacity(TankInfo info);

signals :
    int tryQueryBankInfo(int shipId , int tankId,float sounding);

private:

    float sounding ;
    float temprature ;
    float trim ;

    float capacity;

    int shipId ;

    QLabel *labelName ;
    QLabel *labelEqual ;
    QLineEdit *lineEditSounding ;
    QLineEdit *lineEditTemrature;
    QLineEdit *lineEditTrim;
    QLineEdit *lineEditCapacity;

    //layout
    QHBoxLayout *mainLayout;



};

#endif // TANK_H
