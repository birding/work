#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

#define FILEMANAGER_VERSION "FileManager V0.0.1"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(FILEMANAGER_VERSION);
    ui->pushButton->setText("DISK: ");
    //ui->textEditDir->setText("C:/");
    ui->textEditDir->setText("F:/BDI/");
    ui->textEdit_diskname->setText("Disk1");

    //QObject::connect(&BuildThreadA,SIGNAL(mySignal(QString)),this,SLOT(mySlot(QString)));
    //QObject::connect(&BuildThreadA,SIGNAL(mySignal(QString)),this,SLOT(getProgress(QString)));
    QObject::connect(&dinfo,SIGNAL(sBuildProgress(QString)),this,SLOT(getProgress(QString)));
    QObject::connect(&dinfo,SIGNAL(sBuildProgressDone()),this,SLOT(getProgressDone()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getExistingDirectory(this,"请选择文件夹..."," ");
    qDebug() << file_name;

    ui->textEdit_diskname->setText("Disk1");
    ui->textEditDir->setText(file_name);
}

void MainWindow::on_pB_Gen_clicked()
{

    QString srcDir = ui->textEditDir->toPlainText();
    QString dstDiskName = ui->textEdit_diskname->toPlainText();

    ui->pB_Gen->setEnabled(0);
    if(BuildThreadA.isRunning())
    {
        ui->textBrowser->append("generator thread is running!");
        return ;
    }

    ui->textBrowser->append(QDateTime::currentDateTime().toString("## yyyy-MM-dd hh:mm:ss ddd ##"));
    ui->textBrowser->append(srcDir);

    if(false == dinfo.DirCheck(srcDir))
    {
        ui->textBrowser->append(srcDir + " is not a directory!");
        return;
    }
    ui->textBrowser->append(QString("input DIR is detected:%1").arg(srcDir));
    qDebug() << "srcDir:" + srcDir;

    dinfo.CreateDB();
    QString destDir = dinfo.CreateDisk(srcDir, dstDiskName);
    ui->textBrowser->append(QString("DestDir:%1").arg(destDir));
    qDebug() << "srcDir:" + destDir;

    //dinfo.BuildDisk(0);

    BuildThreadA.setMessage("A");
    BuildThreadA.setDiskInfo(&dinfo);
    BuildThreadA.start();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closing";
    if(BuildThreadA.isRunning())
    {
        qDebug() << "thread is running";
        //QObject::disconnect(&dinfo,SIGNAL(BuildProgress(QString)),this,SLOT(getProgress(QString)));
        BuildThreadA.stop();
        BuildThreadA.wait();
    }
    qDebug() << "closed";
    event->accept();
}

void MainWindow::mySlot(QString message)
{
    qDebug() << "aa:" << message;
}

void MainWindow::getProgress(QString message)
{
    ui->textBrowser->append(message);
}

void MainWindow::getProgressDone()
{
    ui->textBrowser->append("done!");
    ui->textBrowser->append(QDateTime::currentDateTime().toString("## yyyy-MM-dd hh:mm:ss ddd ##"));
    ui->textBrowser->append("\n\n");
    ui->pB_Gen->setEnabled(true);
}
