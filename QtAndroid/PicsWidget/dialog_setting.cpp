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

    ui->comboBox_invl->addItem(tr("2"));
    ui->comboBox_invl->addItem(tr("5"));
    ui->comboBox_invl->addItem(tr("10"));
    ui->comboBox_invl->addItem(tr("20"));
    ui->comboBox_invl->setCurrentIndex(1);
}

Dialog_setting::~Dialog_setting()
{
    delete ui;
}

void Dialog_setting::setMainWindows(MainWindow * rmw)
{
    mw = rmw;
}

void Dialog_setting::RebuildPicsConfig()
{
    QString path = ui->lineEdit_cfgDir->text();
    qDebug("[%d]%s", __LINE__, qPrintable(path));
    if(QDir(path).exists()){
        mw->pConFile->CfgDir = path;
    }

    QString dirCfgFile = path + mw->pConFile->dirFile;
    if(false == QFile::exists(dirCfgFile))
    {
        QMessageBox::information(NULL,"rebuild config","check cfgfile failed!");
        return ;
    }

    QFile file(dirCfgFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(NULL,"rebuild config","open cfgfile failed!");
         return;
    }

    QString picsFile = path + mw->pConFile->picsCfgFile;
    //qDebug()<< "Filepath:" << picsFile;
    QFile fileout( picsFile );
    if (!fileout.open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::information(NULL,"rebuild config","open picsfile failed!");
         return;
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

void Dialog_setting::on_pushButton_update_clicked()
{
    int invl = ui->comboBox_invl->currentText().toInt();
    qDebug("[%d]%d", __LINE__, invl);
    mw->pThread->interval = invl*1000;
}

void Dialog_setting::on_pushButton_build_clicked()
{
    ui->label_percent->setText("start...");
    ui->pushButton_build->setEnabled(false);
    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_cfgDir->setEnabled(false);
    ui->pushButton_Done->setEnabled(false);
    ui->pushButton_update->setEnabled(false);
    RebuildPicsConfig();
    ui->label_percent->setText("done");
    ui->pushButton_build->setEnabled(true);
    ui->pushButton_cancel->setEnabled(true);
    ui->pushButton_cfgDir->setEnabled(true);
    ui->pushButton_Done->setEnabled(true);
    ui->pushButton_update->setEnabled(true);
}
