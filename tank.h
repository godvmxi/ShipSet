#ifndef TANK_H
#define TANK_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QString>
#include "common.h"
#include <QPushButton>
#include <QRegExpValidator>


namespace Ui {
class Tank;
}

class Tank : public QWidget
{
    Q_OBJECT

public:
    explicit Tank(QWidget *parent = 0);
    ~Tank();
    float getCapicaity();
    void setTankTrim(float value);
    void setShipId(int id);
    void setTankId(int id);
    void setTankName(QString name);
    void  setTankCapacity(TankInfo info);

signals :
    int tryQueryBankInfo(int shipId , int tankId,int sounding);

public slots:
    bool setTankInfo(TankInfo info);
    bool updateTankInfo(void);
private:

    int sounding ;
    float temprature ;
    float trim ;

    float capacity;

    int shipId ;
    int tankId;
    QString tankName;

    QLabel *labelName ;
    QLabel *labelEqual ;
    QLineEdit *lineEditSounding ;
    QLineEdit *lineEditTemrature;
    QLineEdit *lineEditTrim;
    QLineEdit *lineEditCapacity;
    QPushButton *pushButtonEqual;

    //layout
    QHBoxLayout *mainLayout;
    QRegExpValidator *regExpValidatorSounding;
    QRegExpValidator *regExpValidatorTemperature;

};

#endif // TANK_H
