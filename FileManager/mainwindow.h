#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QThread>
#include <QCloseEvent>
#include "workthread.h"
#include "diskinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void mySlot(QString message);
    void getProgress(QString message);
    void getProgressDone();

private slots:
    void on_pushButton_clicked();
    void on_pB_Gen_clicked();

private:
    Ui::MainWindow *ui;
    WorkThread BuildThreadA;
    DiskInfo dinfo;

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
