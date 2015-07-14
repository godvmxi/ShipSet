#include "common.h"
#include <QDebug>

void showTankInfo(TankInfo *info){
//    return ;
    qDebug()<<"Show tank info ";
    qDebug()<<"shipId         "<< info->shipId;

    qDebug()<<"tank id       "<< info->tankId;
    qDebug()<<"souding       "<< info->sounding;
    qDebug()<<"type       "<< info->soundingType;
    qDebug()<<"value       "<< info->strValue;

    fprintf(stderr,"capcity -> \n");
    for(int i = 0 ;i<13;i++){
        fprintf(stderr,"%2.2f ",info->capacity[i]);
    }
    qDebug()<<"\n==============";
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
bool convertStringValueToList(float *value,QString source,int maxNum){
  //  qDebug()<<"convert string -> "<<source;
    QStringList list =  source.split("_");
    int listCount =  list.count();
    //qDebug()<<"string deal"<< listCount << maxNum;
    assert(listCount <= maxNum);
    assert(value !=  NULL);
    for (int i = 0 ;i < listCount ;i++){
        value[i] = list.at(i).toFloat();
    }
    return true;
}

void initTankInfo(TankInfo *info){
    info->soundingType = 0;
    info->strValue = "" ;
    for (int i = 0;i < MAX_CAPACITY_PER_TANK ;i++)
        info->capacity[i] = 0;
}
