#include <QCoreApplication>
#include <QProcess>
#include<QDebug>


int main(int argc, char *argv[])
{
#if 0
    qDebug()<<"hello qDebug";
    qWarning()<<"hello qWarning";
    qCritical()<<"hello qCritical";
//    qFatal()   <<"hello qFatal"; //can use like this
    qFatal("hello qFatal");
#endif

    QCoreApplication a(argc, argv);
    QProcess *process = new QProcess();
    process->start("ShipSet.exe");
    if( process->waitForStarted(3000)  ){
        qDebug()<<"start ok";
        process->waitForFinished();
    }
    delete process;
}
