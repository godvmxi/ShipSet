#include "dialogabout.h"
#include "ui_dialogabout.h"
#include <QPixmap>
#include <QDebug>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    //set backgroud
    this->pixmapBackgroud = new QPixmap();
    this->pixmapBackgroud->load(":/res/res/bk0.jpg");
    if(this->pixmapBackgroud->isNull()){
        QMessageBox::critical(NULL, QString::fromUtf8("船只数据库损坏"), QString::fromUtf8("程序资源丢失2"), QMessageBox::Yes, QMessageBox::Yes);
        exit(0);
    }
//    this->setAutoFillBackground(true);
    QPalette    palette = this->palette();
    palette.setBrush(this->backgroundRole(),
                     QBrush(this->pixmapBackgroud->scaled(this->size(),
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation)));
    this->setPalette(palette);

}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButtonAccept_clicked()
{
    this->accept();
}
