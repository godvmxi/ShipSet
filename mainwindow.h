#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tank.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sqlutils.h>
#include <QString>
#include <QVector>
#include "common.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    sqlUtils *sqlCore;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layoutCommon ;

    int shipNumber;
    int shipId;
    ShipInfo shipInfo;

//    QVector<Tank> widgetTankItems;
    size_t widgetTankItems[24];



};

#endif // MAINWINDOW_H
