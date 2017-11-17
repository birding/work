#include "dialog_setting.h"
#include "ui_dialog_setting.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

Dialog_setting::Dialog_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_setting)
{
    ui->setupUi(this);

    ui->comboBox_invl->addItem(tr("1"));
    ui->comboBox_invl->addItem(tr("2"));
    ui->comboBox_invl->addItem(tr("5"));
    ui->comboBox_invl->addItem(tr("10"));
    ui->comboBox_invl->addItem(tr("20"));
}

void Dialog_setting::setGobalEnv(picthread * pTh, configfile * pCF)
{
    pThread = pTh;
    pConFile = pCF;

    int invl = pThread->interval;

    if(invl == 1)
        ui->comboBox_invl->setCurrentIndex(0);
    else if(invl == 2)
        ui->comboBox_invl->setCurrentIndex(1);
    else if(invl == 5)
        ui->comboBox_invl->setCurrentIndex(2);
    else if(invl == 10)
        ui->comboBox_invl->setCurrentIndex(3);
    else if(invl == 20)
        ui->comboBox_invl->setCurrentIndex(4);
    else
        ui->comboBox_invl->setCurrentIndex(2);
}

Dialog_setting::~Dialog_setting()
{
    delete ui;
}

int Dialog_setting::RebuildPicsConfig()
{
    QString path = ui->lineEdit_cfgDir->text();
    //qDebug("[%d]%s", __LINE__, qPrintable(path));
    if(QDir(path).exists()){
        pConFile->CfgDir = path;
    }

    QString dirCfgFile = path + pConFile->dirFile;
    //qDebug("[%d]%s", __LINE__, qPrintable(dirCfgFile));
    if(false == QFile::exists(dirCfgFile))
    {
        //QMessageBox::information(NULL,"rebuild config","check cfgfile failed!");
        ui->label_percent->setText("check cfgfile failed!");
        return -1;
    }

    QFile file(dirCfgFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //QMessageBox::information(NULL,"rebuild config","open cfgfile failed!");
        ui->label_percent->setText("open cfgfile failed!");
         return -1;
    }

    QString picsFile = path + pConFile->picsCfgFile;
    //qDebug()<< "Filepath:" << picsFile;
    QFile fileout( picsFile );
    if (!fileout.open(QIODevice::ReadWrite | QIODevice::Text)){
        //QMessageBox::information(NULL,"rebuild config","open picsfile failed!");
        ui->label_percent->setText("open picsfile failed!");
         return -1;
    }
    QTextStream fsout(&fileout);

    while (!file.atEnd()) {
         QString line = file.readLine();
         line.replace(QString("\n"), QString(""));

         QDir dDir(line);
         //qDebug("[%d]dir %s", __LINE__, line.toLocal8Bit().constData());
         //qDebug("dir count %d", dDir.count());
         if(false == dDir.exists()){
             continue;
         }
         ui->label_percent->setText("checking " + line);

         foreach(QFileInfo mfi , dDir.entryInfoList())
         {
             if(mfi.isFile())
             {
                 fsout << mfi.absoluteFilePath() << endl;
                 //qDebug("pic %s", mfi.absoluteFilePath().toLocal8Bit().constData());
             }
         }
     }
     fsout.flush();
     fileout.close();
     file.close();
     return 0;
}

void Dialog_setting::on_Dialog_setting_accepted()
{
    qDebug("dlg accepted");
}

void Dialog_setting::on_pushButton_cfgDir_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("cfg file direction:"));
    fileDialog->setFileMode(QFileDialog::Directory);

    QString path = fileDialog->getExistingDirectory(NULL, "cfg:", ".");
    if (!path.isEmpty()){
        qDebug("[%d]%s", __LINE__, qPrintable(path));
        ui->lineEdit_cfgDir->setText(path);
    }
}

void Dialog_setting::on_pushButton_Reload_clicked()
{
    sig_Setting_ret(Setting_EVENT_Reload_pics,0);
}

void Dialog_setting::on_pushButton_close_clicked()
{
    sig_Setting_ret(Setting_EVENT_CLOSE,0);
}

void Dialog_setting::on_pushButton_build_clicked()
{
    ui->label_percent->setText("start...");
     ui->pushButton_build->setEnabled(false);
     ui->pushButton_close->setEnabled(false);
     ui->pushButton_cfgDir->setEnabled(false);
     ui->pushButton_Reload->setEnabled(false);
     ui->pushButton_update->setEnabled(false);

     int ret = RebuildPicsConfig();
     if(ret == 0){
         ui->label_percent->setText("done");
     }

     ui->pushButton_build->setEnabled(true);
     ui->pushButton_close->setEnabled(true);
     ui->pushButton_cfgDir->setEnabled(true);
     ui->pushButton_Reload->setEnabled(true);
     ui->pushButton_update->setEnabled(true);
}

void Dialog_setting::on_pushButton_update_clicked()
{
    int invl = ui->comboBox_invl->currentText().toInt();
    sig_Setting_ret(Setting_EVENT_Update_invl, invl);
}
