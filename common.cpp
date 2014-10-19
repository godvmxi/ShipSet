#include "common.h"
#include <QDebug>

void showTankInfo(TankInfo *info){
    qDebug()<<"Show ship info ";
    qDebug()<<"shipId         "<< info->shipId;

    qDebug()<<"shipName       "<< info->tankId;
    qDebug()<<"tankNumber     "<< info->sounding;
    for(int i = 0 ;i<13;i++){
        qDebug()<<"capcacity_"<< i << info->capacity[i];
    }
    qDebug()<<"==============";
}
void showShipInfo(ShipInfo *info){
    qDebug()<<"Show ship info ";
    qDebug()<<"shipId         "<< info->shipId;
    qDebug()<<"crt            "<< info->crt;
    qDebug()<<"shipName       "<< info->shipName;
    qDebug()<<"tankNumber     "<< info->tankNumber;
    qDebug()<<"capacityNumber "<< info->capacityNumber;
    qDebug()<<"shipTrimMin    "<< info->shipTrimMin;
    qDebug()<<"shipTrimStep   "<< info->shipTrimStep;
    qDebug()<<"finaldate      "<< info->finalDate;
    qDebug()<<"==============";
}
