#include <QCoreApplication>
#include <QProcess>
#include<QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QProcess *process = new QProcess();
    process->start("ShipSet.exe");
    if( process->waitForStarted(3000)  ){
        qDebug()<<"start ok";
        process->waitForFinished();

    }
    delete process;
}
