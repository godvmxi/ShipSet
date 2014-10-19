#ifndef COMMON_H
#define COMMON_H
#include <QString>
#define MAX_CAPACITY_PER_TANK    20
typedef struct {
    QString crt;
    QString shipName;
    int tankNumberr;
    int capacityNumber;
    float shipTrimMin;\
    float shipStep;
    QString finalDate;
}ShipInfo;

typedef struct {
    int tankId;
    float sounding;
    float  capacity[MAX_CAPACITY_PER_TANK];
}TankInfo;


#endif // COMMON_H
