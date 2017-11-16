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

    //qDebug("this width %d height %d", this->width(),this->height());
    ui->setupUi(this);

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
    pThread = new picthread();
    ui->pushButton_play->setText("pause");
    pThread->state = true;
    //sqDebug("[%d]", __LINE__);
    pThread->start();
    QObject::connect(pThread,SIGNAL(updateSignal()),this,SLOT(getUpdateEvent()));
}

void MainWindow::getUpdateEvent()
{
    //qDebug("slot >> get updateSignal");
    loadimage();
}

int MainWindow::loadimage()
{
    QString picFile = pConFile->getNextPic();
    QString labtxt = QString::number(pConFile->picIndex)
                        + "/"
                        + QString::number(pConFile->picCount);

    ui->label_ind->setText(labtxt);
    ui->label_ind->repaint();

    //qDebug("[%d]%s", __LINE__, qPrintable(picFile));
    if(false == QFile::exists(picFile))
    {
        QMessageBox::information(NULL,"load img","check file failed!");
        return 0;
    }

    if(picFile.contains(".jpg")|| picFile.contains(".bmp")
            || picFile.contains(".png"))
    {

    }else
    {
        qDebug("[%d]%s is not a picture", __LINE__, picFile.toLocal8Bit().constData());
        return 0;
    }

    QPixmap pixmap;
    if(! ( pixmap.load(picFile) ) ) {
                QMessageBox::information(NULL,"load image","pic failed!");
                setThreadState(false);
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
    setThreadState(false);

    qDebug("setting begin");
    Dialog_setting setting;

    setting.setMainWindows(this);

    if ( setting.exec() == QDialog::Accepted)
    {
        qDebug("[%d] %s", __LINE__, qPrintable(this->pConFile->CfgDir));
        qDebug("[%d] %d", __LINE__, this->pThread->interval);

        pConFile->reload();
        qDebug("reload done");
    }
    qDebug("end");
    setThreadState(true);
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
