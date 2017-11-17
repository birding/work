#ifndef DIALOG_SETTING_H
#define DIALOG_SETTING_H

#include <QDialog>
#include "configfile.h"
#include "picthread.h"

#define Setting_EVENT_CLOSE 1
#define Setting_EVENT_Reload_pics 2
#define Setting_EVENT_Update_invl 3

class MainWindow;

namespace Ui {
class Dialog_setting;
}

class Dialog_setting : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_setting(QWidget *parent = 0);
    ~Dialog_setting();
    void setGobalEnv(picthread * pThread, configfile * pConFile);
    int RebuildPicsConfig();

signals:
    sig_Setting_ret(int event, int val);

private slots:
    void on_Dialog_setting_accepted();

    void on_pushButton_cfgDir_clicked();

    void on_pushButton_Reload_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_build_clicked();

    void on_pushButton_update_clicked();

private:
    Ui::Dialog_setting *ui;
    picthread * pThread;
    configfile * pConFile;
};

#endif // DIALOG_SETTING_H
