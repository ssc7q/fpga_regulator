#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T09:25:47
#
#-------------------------------------------------

message("Platform:")
qnx-i386-g++{
DEFINES += QNX
CONFIG += QNX
CONFIG += DEBUG
message("QNX")
}
win32{
DEFINES += WIN
CONFIG += WIN
message("WIN32")
}


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
    lib.c \
    PCI1713.cpp \
    PulseGeneratorController.cpp \
    PortReaderWriter.cpp

HEADERS  += mainwindow.h \
    mythread.h \
    qcustomplot.h \
    PCI1753.h \
    ioadm.h \
    io-adm-msg.h \
    port_io_msg.h \
    PCI1713.h \
    adc1713_msg.h \
    PulseGeneratorController.h \
    PortReaderWriter.h

FORMS    += mainwindow.ui
