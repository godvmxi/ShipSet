#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.show();

    return a.exec();
}
