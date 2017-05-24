#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "qcustomplot.h"
#include <mythread.h>
#include <QFile>
#include <QTextStream>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    //ui->plot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));

    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->yAxis2, SLOT(setRange(QCPRange)));

    QFile file("out.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        ui->console->insertPlainText("Error: out.txt can't be opened");
    out.setDevice(&file);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGraph(double wx, double t)
{
    int frequpdate = 2; //frequancy of update graph
    int weight = 50;
    static double lastpointKey = 0;
    if (t-lastpointKey > frequpdate)
    {
        ui->plot->graph(0)->addData(t, wx);
        ui->plot->graph(0)->removeDataBefore(t-weight);
        ui->plot->graph(0)->rescaleValueAxis();
        lastpointKey = t;
    }
    ui->plot->xAxis->setRange(t+2.5, weight, Qt::AlignRight);
    ui->plot->replot();

    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (t-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(t-lastFpsKey), 0, 'f', 0)
            .arg(ui->plot->graph(0)->data()->count())
            , 0);
      lastFpsKey = t;
      frameCount = 0;
    }
}

void MainWindow::on_bStart_clicked()
{
    MyThread *thread = new MyThread();
    thread->start();
    connect(thread, SIGNAL(send(double, double)), this, SLOT(updateGraph(double, double)));
    connect(thread, SIGNAL(send(double, double)), this, SLOT(writeToFile(double, double)));

}

void MainWindow::on_bStop_clicked()
{
}

void MainWindow::writeToFile(double wx, double t)
{
    out << wx << "  "<< t << endl;
}
