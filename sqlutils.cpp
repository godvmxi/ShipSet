#include "sqlutils.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>

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
        info[i].capacityNumber = query.value(4).toInt();
        info[i].shipTrimMin = query.value(5).toFloat();
        info[i].shipTrimStep = query.value(6).toFloat();
        info[i].finalDate = query.value(7).toDate();
        i++;
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

    while(query.next()) {

        //qDebug()<<query.value(0) << query.value(1)<< query.value(2)<< query.value(3)<< query.value(4)<< query.value(5)<< query.value(6)<< query.value(7);
        info.shipId = query.value(0).toInt();
        info.crtName = query.value(1).toString();
        info.shipName = query.value(2).toString();
        info.tankNumber = query.value(3).toInt();
        info.capacityNumber = query.value(4).toInt();
        info.shipTrimMin = query.value(5).toFloat();
        info.shipTrimStep = query.value(6).toFloat();
        info.finalDate = query.value(7).toDate();
    }
//    showShipInfo(&info);
    return info;
}

bool sqlUtils::queryTankInfo(int shipId,int tankId,int sounding,TankInfo *retInfo){
    TankInfo info ;
    memset(&info,0,sizeof(TankInfo));
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
