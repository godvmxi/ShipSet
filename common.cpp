#include "common.h"
#include <QDebug>

void showTankInfo(TankInfo *info){
    qDebug()<<"Show tank info ";
    qDebug()<<"shipId         "<< info->shipId;

    qDebug()<<"shipName       "<< info->tankId;
  //  qDebug()<<"tankNumber     "<< info->tankNumber;
    for(int i = 0 ;i<13;i++){
        qDebug()<<"capcacity"<<i<<" ->"<< info->capacity[i];
    }
    qDebug()<<"==============";
}
void showShipInfo(ShipInfo *info){
    qDebug()<<"Show ship info ";
    qDebug()<<"shipId         "<< info->shipId;
    qDebug()<<"crt            "<< info->crtName;
    qDebug()<<"shipName       "<< info->shipName;
    qDebug()<<"tankNumber     "<< info->tankNumber;
    qDebug()<<"capacityNumber "<< info->capacityNumber;


    qDebug()<<"sounding ->" << info->soundingLimit[0] << info->soundingLimit[1];
    qDebug()<<"ship trim h";
    for(int i = 0 ;i < info->capacityNumber ;i++){
        fprintf(stderr,"%2.2f ",info->shipTrimH[i]);
    }
    qDebug()<<"\nship trim v";
    for(int i = 0 ;i < info->capacityNumber ;i++){
        fprintf(stderr,"%2.2f ",info->shipTrimV[i]);
    }
    fflush(stderr);
    qDebug()<<"\nfinaldate      "<< info->crtValidDate;
    qDebug()<<"==============";
}
