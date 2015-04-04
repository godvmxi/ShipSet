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
#include <dialogabout.h>
#include <QPixmap>
#include <QFormLayout>
#include <QLineEdit>
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
    void comboBoxShipCrtNameChanged(int index);
    void pushButtonAboutSlot(void);
    void showSoundingQueryError(int tankId);
    void updateWidgetsToolTips(void);

private:
    void addWidgeHeadInfo(void);
    void addWidgeFootInfo(void);
    void updateWidgetTankTrim(void);
    bool queryTankCapacity(int tankId,int sounding,float *info);
    void  addTankItemsTable(bool clearOld);
//    void paintEvent(QPaintEvent *e);
    int getWindowsHeight(void);
    Ui::MainWindow *ui;
    sqlUtils *sqlCore;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layoutCommon ;

    int shipNumber;
    int shipId;
    ShipInfo shipArray[MAX_SHIP_NUMBER];
    ShipInfo oldShipInfo;
    ShipInfo shipInfo;
    float currentShipTrim ;
    float shipTrimMax ;

//    QVector<Tank> widgetTankItems;
    size_t widgetTankItems[24];

    QScrollArea *scrollAreaTankItemsTable;
    QWidget *widgetTankItemsTable ;
    QVBoxLayout *vBoxLayoutTankItemsTable;
    //middle left layout
    //middle right layout

    //middle right widgets
    QWidget *widgetMiddleRightInfo;
    //middle h layout
    QHBoxLayout *hBoxLayoutMiddleMain;
    //middle right form layout
    QFormLayout *formLayoutMiddleRight;
    //middle widgets
    QWidget *widgetMiddleMain;
    //middle right widgets
    QWidget *widgetMiddleRight;

    //widget head info
    QLabel *labelCrtName ;
    QLabel *labelShipName ;
    QComboBox *comboBoxShipCrtName;
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
    QPushButton *pushButtonAbout;
    //backgroud
    QPixmap *pixmapBackgroud;
    //status bar
    QLabel *labelError ;

    QLineEdit *lineEditTrimH;
    QLineEdit *lineEditTrimV;
    QLineEdit *lineEditOil;
    QLineEdit *lineEditDensity;
    QLineEdit *lineEditVolume ;
    QLineEdit *lineEditTotalCapity ;

};

#endif // MAINWINDOW_H
