#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QDate>
//#pragma execution_character_set("utf-8")
#define MAX_CAPACITY_PER_TANK    40
#define MAX_SHIP_NUMBER          40

typedef struct {
    int shipId;
    QString crt;
    QString shipName;
    int tankNumber;
    int capacityNumber;
    float shipTrimMin;
    float shipTrimStep;
    QDate finalDate;
}ShipInfo;

typedef struct {
    int shipId;
    int tankId;
    int sounding;
    float  capacity[MAX_CAPACITY_PER_TANK];
}TankInfo;
void showTankInfo(TankInfo *info);
void showShipInfo(ShipInfo *info);

#endif // COMMON_H
