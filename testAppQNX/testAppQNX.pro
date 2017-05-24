#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T09:25:47
#
#-------------------------------------------------
CONFIG += debug
TARGET = testAppQNX
TEMPLATE = app
QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

target.files = testAppQNX
target.path = /root/testButton

INSTALLS += target


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp \
    qcustomplot.cpp \
    PCI1753.cpp \
    lib.c

HEADERS  += mainwindow.h \
    mythread.h \
    qcustomplot.h \
    PCI1753.h \
    ioadm.h \
    io-adm-msg.h \
    port_io_msg.h

FORMS    += mainwindow.ui
