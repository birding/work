#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include "picthread.h"
#include "configfile.h"
#include "dialog_setting.h"

#define PICSWIDGET_VERSION "0.0.2"  //separate the setting dialogue

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
    Dialog_setting * pSetting;
    ~MainWindow();
    void setThreadState(int on);

private slots:
    void on_pushButton_pre_clicked();
    void getUpdateEvent();
    void handle_Setting_Event(int event, int val);

    void on_pushButton_play_clicked();

    void on_pushButton_Setting_clicked();

    void on_pushButton_next_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
