#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include "picthread.h"
#include "configfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int loadimage();
    int loadimage_fromQImage();
    QLabel * picLabel;
    picthread * pThread;
    configfile * pConFile;
    ~MainWindow();
    void setThreadState(int on);

private slots:
    void on_pushButton_pre_clicked();
    void getUpdateEvent();

    void on_pushButton_play_clicked();

    void on_pushButton_Setting_clicked();

    void on_pushButton_next_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
