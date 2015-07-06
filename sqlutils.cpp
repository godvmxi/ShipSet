#include "sqlutils.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <assert.h>

sqlUtils::sqlUtils(QObject *parent) :
    QObject(parent)
{
   this->db =  QSqlDatabase::addDatabase("QSQLITE");
    //qDebug()<<this->db.drivers();
}
int sqlUtils::setDbFile(QString fileName){

    this->db.setDatabaseName(fileName);

    if(!this->db.open() )
    {
        QMessageBox::critical(NULL, QObject::tr("Collection"), QObject::tr("failed to connect to database!"));
        exit(1);
    }
    //qDebug()<<"open db success";





   if (this->db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        //qDebug()<<"support size ";

   }
   else{
        //qDebug()<<"support not size ";
   }

    return true;
}
bool convertStringValueToList(float *value,QString source,int maxNum){
//    qDebug()<<"convert string -> "<<source;
    QStringList list =  source.split("_");
    int listCount =  list.count();
//    qDebug()<<"string deal"<< listCount << maxNum;
    assert(listCount <= maxNum);
    assert(value !=  NULL);
    for (int i = 0 ;i < listCount ;i++){
        value[i] = list.at(i).toFloat();
    }
    return true;
}

bool sqlUtils::queryShipsInfo(ShipInfo *info){
    if(info == NULL)
        return false;
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM shipInfo ORDER BY shipId");
    query.exec(sql);
    int i = 0;
    while(query.next()) {
        //qDebug()<<query.value(0) << query.value(1)<< query.value(2)<< query.value(3)<< query.value(4)<< query.value(5)<< query.value(6)<< query.value(7);
        info[i].shipId = query.value(0).toInt();
        info[i].crtName = query.value(1).toString();
        info[i].shipName = query.value(2).toString();
        info[i].tankNumber = query.value(3).toInt();
        convertStringValueToList(info->soundingLimit,query.value(4).toString(),2);
        info[i].capacityNumber = query.value(5).toInt();


        convertStringValueToList(info->shipTrimH,query.value(6).toString(),MAX_CAPACITY_PER_TANK);

        convertStringValueToList(info->shipTrimV,query.value(7).toString(),MAX_CAPACITY_PER_TANK);
        info->crtValidDate = query.value(8).toDate();
    }
    return true;
}

ShipInfo sqlUtils::queryShipInfo(int shipId){
    ShipInfo info ;
    memset(&info,0,sizeof(ShipInfo));
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM shipInfo where shipId = %1").arg(shipId);
//    qDebug()<< sql;
    query.exec(sql);
    QString temp ;
    while(query.next()) {

        //qDebug()<<query.value(0) << query.value(1)<< query.value(2)<< query.value(3)<< query.value(4)<< query.value(5)<< query.value(6)<< query.value(7);
        info.shipId = query.value(0).toInt();
        info.crtName = query.value(1).toString();
        info.shipName = query.value(2).toString();
        info.tankNumber = query.value(3).toInt();
        temp = query.value(4).toInt();
        info.capacityNumber = query.value(4).toInt();
        info.shipTrimMin = query.value(6).toFloat();
        info.shipTrimStep = query.value(7).toFloat();
        info.crtValidDate = query.value(8).toDate();
    }
//    showShipInfo(&info);
    return info;
}
int sqlUtils::queryTankReferSounding(TankInfo *info,int soundingMax,int soundingMin){
    int soundingStep = 5000;
    int sounding = info[0].sounding;
    int soundingRef1 = sounding - soundingStep >=soundingMin ? sounding - soundingStep :\
                                                                soundingMin;
    int soundingRef2 = sounding + soundingStep <=soundingMax ? sounding + soundingStep :\
                                                                soundingMax;
    qDebug()<<"query->" << soundingMin<<soundingMax;
    QString sql = QString("SELECT sounding FROM tankInfo where shipId = %1 and tankId = %2 and sounding >= %3  and sounding <= %4")\
            .arg(info->shipId).arg(info->tankId).arg(soundingRef1).arg(soundingRef2);
    QSqlQuery query(this->db);
    query.exec(sql);



    return true;
}
bool sqlUtils::queryTankValue(int shipId,int tankId ,int sounding,int queryType,TankInfo *retInfo){

    qDebug()<<__func__<<shipId<<tankId<<sounding<<queryType;
    TankInfo temp[MAX_RESULT_PER_SQL] = {0};
//    memset(temp,0,MAX_RESULT_PER_SQL);
    QSqlQuery query(this->db);
    QString sql ;
    int rangeSize[] =  {10,100,500,1000,2000,5000};
    qDebug()<<"range size ->%s" << sizeof(rangeSize);
    int sounding_min = 0;
    int sounding_max = 0;
    for (int i = 0 ;i < sizeof(rangeSize);i++){
        sounding_min =  sounding  - rangeSize[i];
        if (sounding_min < 0 )
            sounding_min = 0;
        sounding_max = sounding  + rangeSize[i] ;
        sql = QString("SELECT * FROM tankInfo where shipId = %1 and tankId = %2 and sounding >=  %3 and sounding <= %4 and soundingType = %5")
                .arg(shipId).arg(tankId).arg(sounding_min).arg(sounding_max).arg(queryType);
        qDebug()<<sql;
        query.exec(sql);
        int affectLines = 0;

        while(query.next()) {
            temp[affectLines].shipId = query.value(0).toInt();
            temp[affectLines].tankId = query.value(1).toInt();
            temp[affectLines].sounding = query.value(2).toInt();
            temp[affectLines].soundingType = query.value(3).toInt();
            temp[affectLines].strValue = query.value(4).toString();
            qDebug()<<"sql value--> "<<query.value(0).toInt() <<query.value(1).toInt()
                  <<query.value(2).toInt()
                    <<query.value(3).toInt()
                   <<temp[affectLines].strValue;
            affectLines++;
           }
        qDebug()<<"sql result--> " << affectLines;
        int matchFlag = 0;
        int min ,max = 0;
        for (int index = 1;(index < MAX_RESULT_PER_SQL )&&(index < affectLines) ;index++){
            if (sounding == temp[index].sounding ){
                min  = index ;
                max = index ;
                matchFlag = 1;
                qDebug()<<"find match value 1 -> " << min << sounding;
                break;
            }
            else if (sounding == temp[index-1].sounding ){
                min  = index - 1 ;
                max = index - 1 ;
                matchFlag = 1;
                qDebug()<<"find match value 2 -> " << min <<sounding ;
                break;
            }
            else if (sounding > temp[index-1].sounding && sounding < temp[index].sounding){
                min  = index -  1;
                max = index ;
                matchFlag =  2;
                qDebug()<<"find match value 3 -> " << min << max << temp[index-1].sounding << temp[index].sounding;
                break;
            }
        }
        if  (matchFlag > 0 ){
            break;
        }
        else {
            qDebug()<<"try to enlarge the range ->"<<rangeSize[i+1];
        }

    }
    return false;
//    QString sql = QString("SELECT * FROM tankInfo where shipId = %1 and tankId = %2 and sounding = %3 and soundingType = %4")
//            .arg(shipId).arg(tankId).arg(sounding).arg(queryType);
    qDebug()<<sql;
    query.exec(sql);

    int affectLines = 0;

    while(query.next()) {
        temp[affectLines].shipId = query.value(0).toInt();
        temp[affectLines].tankId = query.value(1).toInt();
        temp[affectLines].sounding = query.value(2).toInt();
        temp[affectLines].soundingType = query.value(3).toInt();
        temp[affectLines].strValue = query.value(4).toString();
        qDebug()<<"sql value--> "<<query.value(0).toInt() <<query.value(1).toInt()
              <<query.value(2).toInt()
                <<query.value(3).toInt()
               <<temp[affectLines].strValue;
        affectLines++;
       }
    qDebug()<<"sql result--> " << affectLines;

    return false;
}

bool sqlUtils::queryTankInfo(int shipId,int tankId,int sounding,TankInfo *retInfo){
    TankInfo info ;
    queryTankValue(shipId,tankId,sounding,2,&info);
    return false;
    memset(&info,0,sizeof(TankInfo));
    qDebug()<<"try query tank info";
    return false;
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM tankInfo where shipId = %1 and tankId = %2 and sounding = %3").arg(shipId).arg(tankId).arg(sounding);
//    //qDebug()<< sql;
    query.exec(sql);
    int affectLines = 0;

    while(query.next()) {
        affectLines++;
        info.shipId = query.value(0).toInt();
        info.tankId = query.value(1).toInt();
        info.sounding = query.value(2).toInt();
        QString temp = query.value(3).toString();
        QStringList capacitys =  temp.split(" ");
        for (int i = 0;i<capacitys.size();i++){
            info.capacity[i] = capacitys.at(i).toFloat();
        }
    }
//    qDebug()<<"sql affect lines -> "<<affectLines;
//    showTankInfo(&info);
    if(affectLines == 0){
        qDebug("can find target data");
        return false;
    }
    *retInfo = info;
    return true;
//    emit this->reportTankInfo(info);
//    return info;
}

int sqlUtils::queryShipNumber(void){
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM shipInfo");
    //qDebug()<< sql;
    query.exec(sql);
    int result = 0 ;
    while(query.next()) {
        result++;
    }
    //qDebug()<<"ship number -> " <<result;
    return result ;

}
