#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>

int database_open(void)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("my.db");
    if(!db.open()){
        QMessageBox::critical(0,"Cannot open database", "Unable to establish"
                              , QMessageBox::Cancel);
        return false;
    }
}

database::database()
{
    qDebug() << "availabel database driver";
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        qDebug() << driver;
    }

    database_open();
}
