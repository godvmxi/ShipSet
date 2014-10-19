#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tank.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sqlutils.h>
#include <QString>
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
    Tank *widgetTankItems;







};

#endif // MAINWINDOW_H
