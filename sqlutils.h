#ifndef SQLUTILS_H
#define SQLUTILS_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>

class sqlUtils : public QObject
{
    Q_OBJECT
public:
    explicit sqlUtils(QObject *parent = 0);
    int setDbFile(QString fileName);

signals:
    int queryShipInfo();
    int queryTankCapicity(int tankId,float sounding);



public slots:

private :
    QString dbName;
    QSqlDatabase db;

};

#endif // SQLUTILS_H
