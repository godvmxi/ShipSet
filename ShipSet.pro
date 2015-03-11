#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T15:55:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShipSet
TEMPLATE = app
DEFINES += QT_NO_DEBUG_OUTPUT


SOURCES += main.cpp\
        mainwindow.cpp \
    tank.cpp \
    sqlutils.cpp \
    common.cpp \
    dialogabout.cpp

HEADERS  += mainwindow.h \
    tank.h \
    sqlutils.h \
    common.h \
    dialogabout.h

FORMS    += mainwindow.ui \
    dialogabout.ui

RESOURCES += \
    res.qrc

RC_FILE =    myapp.rc




