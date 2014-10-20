#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T15:55:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ship01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tank.cpp \
    sqlutils.cpp \
    common.cpp

HEADERS  += mainwindow.h \
    tank.h \
    sqlutils.h \
    common.h

FORMS    += mainwindow.ui
