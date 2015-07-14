#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include <QTextBrowser>
#include "common.h"
#include <QNetworkInterface>

class QFeedBack : public QObject
{
    Q_OBJECT
public:
    explicit QFeedBack(QObject *parent = 0,QString url=QString("" ));
    void report(QString value );
    QString getSystemDetail(void);
signals:

public slots:

private :
    QNetworkAccessManager *network_manager ;
    QNetworkRequest *network_request ;
    QNetworkReply *network_reply ;
    QString url ;
    QByteArray  post_data;
    QString sysInfo;
    QString hostname;
    QString macs ;
private slots:
    void replyFinished(QNetworkReply *);

};

#endif // FEEDBACK_H
