#include "sqlutils.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>

sqlUtils::sqlUtils(QObject *parent) :
    QObject(parent)
{
   this->db =  QSqlDatabase::addDatabase("QSQLITE");
    qDebug()<<this->db.drivers();
}
int sqlUtils::setDbFile(QString fileName){

    this->db.setDatabaseName(fileName);

    if(!this->db.open() )
    {
        QMessageBox::critical(NULL, QObject::tr("Collection"), QObject::tr("failed to connect to database!"));
        exit(1);
    }
    qDebug()<<"open db success";





   if (this->db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        qDebug()<<"support size ";

   }
   else{
        qDebug()<<"support not size ";
   }

    return true;
}

ShipInfo sqlUtils::queryShipInfo(int shipId){
    ShipInfo info;
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM shipInfo where shipId = %1").arg(shipId);
    qDebug()<< sql;
    query.exec(sql);

    while(query.next()) {

        qDebug()<<query.value(0) << query.value(1)<< query.value(2)<< query.value(3)<< query.value(4)<< query.value(5)<< query.value(6)<< query.value(7);
        info.shipId = query.value(0).toInt();
        info.crt = query.value(1).toString();
        info.shipName = query.value(2).toString();
        info.tankNumber = query.value(3).toInt();
        info.capacityNumber = query.value(4).toInt();
        info.shipTrimMin = query.value(5).toFloat();
        info.shipTrimStep = query.value(6).toFloat();
        info.finalDate = query.value(7).toDate();
    }
    showShipInfo(&info);
    return info;
}

TankInfo sqlUtils::queryTankInfo(int shipId,int tankId,float sounding){
    TankInfo info;
    QSqlQuery query(this->db);
    QString sql = QString("SELECT * FROM tankInfo where shipId = %1").arg(shipId);
    qDebug()<< sql;
    query.exec(sql);


    while(query.next()) {

//        qDebug()<<query.value(0) << query.value(1)<< query.value(2)<< query.value(3)<< query.value(4)<< query.value(5)<< query.value(6)<< query.value(7);
        info.shipId = query.value(0).toInt();
        info.tankId = query.value(1).toInt();
        info.sounding = query.value(2).toInt();
        for(int i = 0;i<13;i++){
            info.capacity[i]  = query.value(3+i ).toInt();
        }
    }
    showTankInfo(&info);
    return info;

}
