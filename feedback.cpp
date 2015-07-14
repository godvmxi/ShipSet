#include "feedback.h"

QFeedBack::QFeedBack(QObject *parent,QString url) :
    QObject(parent)
{
    this->sysInfo =  this->getSystemDetail();
    this->hostname = QHostInfo::localHostName();
    this->url = url;
    this->network_manager = new QNetworkAccessManager(this);
    this->network_request = new QNetworkRequest();
    this->network_request->setHeader(QNetworkRequest::ContentTypeHeader ,"application/x-www-form-urlencoded");
    this->network_request->setHeader(QNetworkRequest::UserAgentHeader,"WareFeed .NET Helper Class/1.0");


    this->connect(this->network_manager,SIGNAL(finished(QNetworkReply*)) , this,SLOT(replyFinished(QNetworkReply*)) ) ;
  //  this->manager->get(QNetworkRequest(this->url)) ;
}

void QFeedBack::replyFinished(QNetworkReply *reply){
    QTextCodec *codec = QTextCodec::codecForName(("utf8")) ;
    QString all = codec->toUnicode(reply->readAll());
    reply->deleteLater();
    qDebug()<<"replay status ->";
    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<status;
    qDebug()<<this->url;



}
void QFeedBack::report(QString value ){
    this->post_data.append(QString("u=%1&").arg(FEEDBACK_USAGE_TYPES));
    this->post_data.append(QString("c=%1&").arg(this->hostname));
    this->post_data.append("v=extradata");
    this->network_request->setUrl(this->url);
    this->network_manager->post(*this->network_request,this->post_data);

}

QString QFeedBack::getSystemDetail(void){
     QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
     QString hostinfo = QHostInfo::localHostName();
     foreach(QNetworkInterface i, list) {
            if (i.flags() & QNetworkInterface::IsLoopBack)
                continue;
            else
            {

                hostinfo = QString("%1#%2").arg(hostinfo).arg(i.hardwareAddress());
            }
         }
//     qDebug()<<hostinfo;
//     qDebug()<<QHostInfo::localHostName();
     return hostinfo;
}
