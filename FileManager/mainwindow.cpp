#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "diskinfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setText("DISK: ");

    ui->textEditDir->setText("F:/BDI");
    ui->textEdit_diskname->setText("Disk1");
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
    DiskInfo dinfo;
    QString srcDir = ui->textEditDir->toPlainText();
    QString dstDiskName = ui->textEdit_diskname->toPlainText();

    ui->textBrowser->setText(srcDir);

    if(false == dinfo.DirCheck(srcDir))
    {
        ui->textBrowser->append(srcDir + " is not a directory!");
        return;
    }
    ui->textBrowser->append("input DIR is detected");

    dinfo.CreateDB();
    QString destDir = dinfo.CreateDisk(dstDiskName);
    qDebug() << "srcDir:" + srcDir;
    dinfo.BuildDisk(srcDir, destDir);

}

