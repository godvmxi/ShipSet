#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tank.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sqlutils.h>
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
    tank *tank_1;
    tank *tank_2;
    tank *tank_3;
    QVBoxLayout *mainLayout;
    QHBoxLayout *layoutCommon ;





};

#endif // MAINWINDOW_H
