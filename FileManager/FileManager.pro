#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T20:35:26
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diskinfo.cpp \
    workthread.cpp

HEADERS  += mainwindow.h \
    diskinfo.h \
    workthread.h

FORMS    += mainwindow.ui

