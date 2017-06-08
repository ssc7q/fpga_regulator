#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "qcustomplot.h"
#include <mythread.h>
#include <QFile>
#include <QTextStream>
#include <QString>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //*** init Graph ***
    ui->eplotY->addGraph();
    ui->eplotY->graph(0)->setPen(QPen(Qt::blue));
}

Dialog::~Dialog()
{
    delete ui;
}
