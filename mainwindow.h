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
#include <QScrollArea>
#include <QComboBox>
#include <QPaintEvent>
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
    void pushButtonCalTotalCapacity(void);
    void comboBoxShipCrtChanged(int index);

private:
    void addWidgeHeadInfo(void);
    void addWidgeFootInfo(void);
    void updateWidgetTankTrim(void);
    float queryTankCapacity(int tankId,int sounding);
    void  addTankItemsTable(bool clearOld);
    Ui::MainWindow *ui;
    sqlUtils *sqlCore;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layoutCommon ;

    int shipNumber;
    int shipId;
    ShipInfo shipArray[MAX_SHIP_NUMBER];

    ShipInfo shipInfo;
    float currentShipTrim ;
    float shipTrimMax ;

//    QVector<Tank> widgetTankItems;
    size_t widgetTankItems[24];

    QScrollArea *scrollAreaTankItemsTable;
    QWidget *widgetTankItemsTable ;
    QVBoxLayout *vBoxLayoutTankItemsTable;

    //widget head info
    QLabel *labelCrt ;
    QLabel *labelShipName ;
    QComboBox *comboBoxShipCrt;
    QLabel *labelTrim ;
    QDoubleSpinBox *doubleSpinBoxTrim ;

    QWidget *widgetHeadInfo;
    QHBoxLayout *hBoxLayoutHeadInfo ;
    //widget table title
    QLabel   *labelTableTitle;
    //widget foot info
    QLabel *labelFinalDate;
    QPushButton *pushButtonTotalCapacity;
    QLabel *labelTotalCapacity;
    QHBoxLayout *hBoxLayoutFootInfo ;
    QWidget *widgetFootInfo;
};

#endif // MAINWINDOW_H
