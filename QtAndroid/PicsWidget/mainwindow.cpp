#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSysInfo>
#include <QDebug>
#include "configfile.h"
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "picthread.h"
#include "dialog_setting.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    pConFile = new configfile();
    pThread = new picthread();
    pSetting = new Dialog_setting();

    //qDebug("this width %d height %d", this->width(),this->height());
    ui->setupUi(this);
    this->setWindowTitle(QString("Pics ") + QString(PICSWIDGET_VERSION));

    picLabel =ui->piclabel;
    picLabel->setAlignment(Qt::AlignCenter);

    ui->comboBox_steps->addItem(tr("1"));
    ui->comboBox_steps->addItem(tr("5"));
    ui->comboBox_steps->addItem(tr("10"));
    ui->comboBox_steps->addItem(tr("20"));
    ui->comboBox_steps->addItem(tr("50"));
    ui->comboBox_steps->addItem(tr("100"));
    ui->comboBox_steps->setCurrentIndex(0);

    qDebug("this width %d height %d", this->width(),this->height());
    qDebug("picLabel width %d height %d", picLabel->width(),picLabel->height());
    loadimage();

    //qDebug("[%d]", __LINE__);
    ui->pushButton_play->setText("pause");
    pThread->state = true;
    pThread->interval = 2;
    pThread->start();

    pSetting->setGobalEnv(pThread, pConFile);
    pSetting->setWindowFlags(pSetting->windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
    pSetting->setWindowFlags(pSetting->windowFlags()|Qt::WindowStaysOnTopHint);

    QObject::connect(pThread,SIGNAL(updateSignal()),this,SLOT(getUpdateEvent()));
    QObject::connect(pSetting,SIGNAL(sig_Setting_ret(int, int)),this,SLOT(handle_Setting_Event(int, int)));

}

void MainWindow::getUpdateEvent()
{
    //qDebug("slot >> get updateSignal");
    loadimage();
}

void MainWindow::handle_Setting_Event(int event, int val)
{
    //qDebug("slot >> get setting event %d", event);
    if(event == Setting_EVENT_CLOSE)
    {
        ui->pushButton_Setting->setEnabled(true);
        return ;
    }

    if(event == Setting_EVENT_Reload_pics)
    {
        setThreadState(false);

        pConFile->reload();
        qDebug("reload done");
        loadimage();
        setThreadState(true);
        return ;
    }

    if(event == Setting_EVENT_Update_invl)
    {
        setThreadState(false);

        pThread->interval = val;
        qDebug("interval %d done", val);

        setThreadState(true);
        return ;
    }
    return ;
}

int MainWindow::loadimage()
{
    QString picFile = pConFile->getNextPic();
    QString labtxt = QString::number(pConFile->picIndex)
                        + "/"
                        + QString::number(pConFile->picCount);

    ui->label_ind->setText(labtxt);
    ui->label_ind->repaint();

    if(picFile.contains(".jpg")|| picFile.contains(".bmp")
            || picFile.contains(".png"))
    {

    }else
    {
        qDebug("[%d]%s is not a picture", __LINE__, picFile.toLocal8Bit().constData());
        return 0;
    }

    if(false == QFile::exists(picFile))
    {
        //QMessageBox::information(NULL,"load img","check file failed!");
        ui->piclabel->setText("error: no picfile!");
        return 0;
    }
	
    QPixmap pixmap;
    if(! ( pixmap.load(picFile) ) ) {
                //QMessageBox::information(NULL,"load image","pic failed!");
                ui->piclabel->setText("load picfile failed!");
                qDebug("[%d]%s load picfile failed!", __LINE__, qPrintable(picFile));
                //setThreadState(false);
                return -1;
     }

    QPixmap qScaledpixmap;
    qScaledpixmap = pixmap.scaled(picLabel->width(),picLabel->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    //qDebug("picLabel width %d height %d", picLabel->width(),picLabel->height());
    //qDebug("qScaledpixmap width %d height %d", qScaledpixmap.width(),qScaledpixmap.height());

    picLabel->setPixmap(qScaledpixmap);
    return 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setThreadState(int on)
{
    if(on == true)
    {
        ui->pushButton_play->setText("pause");
        pThread->state = true;
     }else{
        ui->pushButton_play->setText("play");
        pThread->state = false;
    }
}

void MainWindow::on_pushButton_play_clicked()
{
    //qDebug("%s", qPrintable(ui->pushButton_play->text()));
    if(ui->pushButton_play->text() == "play")
    {
        setThreadState(true);
    }
    else
    {
        setThreadState(false);
    }
}

void MainWindow::on_pushButton_Setting_clicked()
{
    //setThreadState(false);
    qDebug("setting begin");

    pSetting->show();
    ui->pushButton_Setting->setEnabled(false);

    qDebug("end");
    //setThreadState(true);
}

void MainWindow::on_pushButton_pre_clicked()
{
    setThreadState(false);
    int steps = ui->comboBox_steps->currentText().toInt();

    this->pConFile->picIndex = this->pConFile->picIndex - steps -1;
    if(this->pConFile->picIndex < 0)
    {
        this->pConFile->picIndex = this->pConFile->picCount - 1;
    }
    if(0 == this->pConFile->picCount)
        this->pConFile->picIndex = 0;
    loadimage();
}

void MainWindow::on_pushButton_next_clicked()
{
    setThreadState(false);
    int steps = ui->comboBox_steps->currentText().toInt();

    this->pConFile->picIndex = this->pConFile->picIndex + steps -1;
    if(this->pConFile->picIndex > this->pConFile->picCount)
    {
        this->pConFile->picIndex = 0;
    }
    loadimage();
}
