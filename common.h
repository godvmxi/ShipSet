#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QDate>
//#pragma execution_character_set("utf-8")
#define MAX_CAPACITY_PER_TANK    40
#define MAX_SHIP_NUMBER          40

typedef struct {
    int shipId;
    QString crtName;
    QString shipName;
    int tankNumber;
    int capacityNumber;
    float soundingLimit[2];
    float shipTrimH[MAX_CAPACITY_PER_TANK];
    float shipTrimV[MAX_CAPACITY_PER_TANK];

    QDate crtValidDate;
    float shipTrimMin;
    float shipTrimStep;
}ShipInfo;

#define  TANK_TRIM_V_VALUE    0
#define  TANK_TRIM_H_VALUE    1
#define  TANK_CAPICITY_VALUE  2
typedef struct {
    int shipId;
    int tankId;
    int sounding;
    int soundingType;
    float  capacity[MAX_CAPACITY_PER_TANK];
    QString strValue;
}TankInfo;
void showTankInfo(TankInfo *info);
void showShipInfo(ShipInfo *info);
#define MAX_RESULT_PER_SQL  100

#endif // COMMON_H
