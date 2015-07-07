#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class DialogAbout;
}

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();

private slots:
    void on_pushButtonAccept_clicked();

private:
    Ui::DialogAbout *ui;
    QPixmap *pixmapBackgroud;
};

#endif // DIALOGABOUT_H
