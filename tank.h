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
    void setTankCapacity(TankInfo info);
    void setTankCapacity(float value);
    void setSoundingLimit(int min,int max);
    bool checkDataValidator(void);

    int getTankId(void);
    int getSounding(void);
    void setTankInvalid(bool enable);
    QString getTankName(void);

signals :
    void tryQueryBankInfo(int shipId , int tankId,int sounding);
    void tryQueryBankInfo(int tankId,int sounding);

public slots:
    bool setTankInfo(TankInfo info);
    void updateTankInfo(void);
private:

    int sounding ;
    int soundingMin;
    int soundingMax ;
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
