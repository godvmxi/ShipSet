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
#include <QDoubleSpinBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots :
    void queryTankInfoSlot(int tankId,int sounding);
    void shipTrimChanged(QString d);

private:
    void addWidgeHeadInfo(void);
    void updateWidgetTankTrim(void);
    float queryTankCapacity(int tankId,int sounding);
    Ui::MainWindow *ui;
    sqlUtils *sqlCore;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layoutCommon ;

    int shipNumber;
    int shipId;

    ShipInfo shipInfo;
    float currentShipTrim ;
    float shipTrimMax ;

//    QVector<Tank> widgetTankItems;
    size_t widgetTankItems[24];


    //widget head info
    QLabel *labelCrt ;
    QLabel *labelShipName ;
    QLabel *labelTrim ;
    QDoubleSpinBox *doubleSpinBoxTrim ;
    QLabel *labelFinalDate;
    QWidget *widgetHeadInfo;
    QHBoxLayout *hBoxLayoutHeadInfo ;
    //widget table title

    //widget result
};

#endif // MAINWINDOW_H
