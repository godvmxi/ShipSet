#ifndef SQLUTILS_H
#define SQLUTILS_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include "common.h"

class sqlUtils : public QObject
{
    Q_OBJECT
public:
    explicit sqlUtils(QObject *parent = 0);
    int setDbFile(QString fileName);


signals:
    void reportShipInfo(ShipInfo info);
    void reportTankInfo(TankInfo info);



public slots:
    int queryShipNumber(void);
    bool queryShipsInfo(ShipInfo *info);
    ShipInfo queryShipInfo(int shipId);
    int queryTankReferSounding(TankInfo *,int soundingMax,int soundingMin);
    bool queryTankInfo(int shipId,int tankId,int sounding,TankInfo *info);
    bool queryTankValueArray(TankInfo *retInfo);
private :
    QString      dbName;
    QSqlDatabase db;

};

#endif // SQLUTILS_H
