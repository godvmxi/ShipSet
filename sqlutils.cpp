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
    QSqlQuery query(this->db);
    qDebug()<<query.exec("SELECT * FROM info ; ");



   if (this->db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        qDebug()<<"support size ";

   }
   else{
        qDebug()<<"support not size ";
   }
   while(query.next()) {
       qDebug()<<query.value(0).toString();
   }
        qDebug()<<"end sqlite";


    return true;
}
