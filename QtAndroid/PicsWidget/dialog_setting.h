#ifndef DIALOG_SETTING_H
#define DIALOG_SETTING_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Dialog_setting;
}

class Dialog_setting : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_setting(QWidget *parent = 0);
    ~Dialog_setting();
    void setMainWindows(MainWindow * mw);
    void RebuildPicsConfig();

private slots:
    void on_Dialog_setting_accepted();

    void on_pushButton_cfgDir_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_build_clicked();

private:
    Ui::Dialog_setting *ui;
    MainWindow * mw;
};

#endif // DIALOG_SETTING_H
