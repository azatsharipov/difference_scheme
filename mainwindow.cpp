#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include "qcustomplot.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->doubleSpinBox->setMaximum(10000);
    ui->doubleSpinBox->setValue(0.05);
    ui->doubleSpinBox_2->setMaximum(10000);
    ui->doubleSpinBox_2->setValue(0.1);
    ui->doubleSpinBox_3->setMaximum(10000);
    ui->doubleSpinBox_3->setValue(100);
    ui->label_3->setVisible(false);
    ui->doubleSpinBox_3->setVisible(false);
    ui->doubleSpinBox_4->setMaximum(10000);
    ui->doubleSpinBox_4->setValue(1);
    ui->doubleSpinBox_5->setMaximum(10000);
    ui->doubleSpinBox_5->setValue(100);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::generate() {
    tau = ui->doubleSpinBox->value();
    h = ui->doubleSpinBox_2->value();
    u0 = ui->doubleSpinBox_3->value();
    v = ui->doubleSpinBox_4->value();
    tMax = ui->doubleSpinBox_5->value();

    double xMax = tMax;
    x.clear();
    y.clear();
    y2.clear();
    for (double t = -xMax; t < 0; t++) {
        x.push_back(t);
        y.push_back(0);
        y2.push_back(0);
    }
    int w = 0;
    if (w == 0) {
        // sin
        for (double t = 0; t <= xMax; t++) {
            x.push_back(t);
            y.push_back(sin(t * pi / xMax));
            y2.push_back(sin(t * pi / xMax));
            qDebug() << x[x.size() - 1] << y[y.size() - 1];
        }
    } else {
        // ladder
        for (double t = 0; t <= xMax; t++) {
            x.push_back(double(int(t + 1) / 2 * 2));
            y.push_back(double(int(t) / 2 * 2));
            y2.push_back(double(int(t) / 2 * 2));
            qDebug() << x[x.size() - 1] << y[y.size() - 1];
        }
    }
    for (double t = xMax + 1; t <= 2 * xMax; t++) {
        x.push_back(t);
        y.push_back(0);
        y2.push_back(0);
    }
    stepNumber = 0;
    draw();
}

double MainWindow::getU(double t, double yPre, double yPrePre) {
    return yPre - v * (yPre - yPrePre) * tau / h;
}

double MainWindow::getUCenter(double t, double yPre, double yPrePre, double yNext) {
    return yPre - v * (yNext - yPrePre) * tau / 2 / h;
}

double MainWindow::getUSquare(double t, double yPre, double yPrePre, double yThis) {
    return (-h * (yThis - yPre - yPrePre) - v * tau * (yPre - yThis - yPrePre)) / (h + v * tau);
}

double MainWindow::getUHopf(double t, double yPre, double yPrePre, double yNext) {
//    return (-h * (yPre - yNext - yPrePre) - yPre * tau * (yNext - yPre - yPrePre)) / (h + yPre * tau);
    return yPre - yPre * (yPre - yPrePre) * tau / h;
}

void MainWindow::step() {
    draw();
}

void MainWindow::draw() {

    double a = 0; //Начало интервала, где рисуем график по оси Ox
    double b = 10000; //Конец интервала, где рисуем график по оси Ox

    QVector<double> yPre, yPre2;
    double xMax = tMax;
    yPre.clear();
    yPre2.clear();
    for (double t = -xMax; t <= 2 * xMax; t++) {
        yPre.push_back(y[t + xMax]);
        yPre2.push_back(y2[t + xMax]);
    }
    if (stepNumber > 0) {
        y.clear();
        y2.clear();
        int currentTau = 1;
        for (double t = -xMax; t <= 2 * xMax; t++) {
            int what = 2;
            if (what == 0) {
                if (t == -xMax) {
                    y.push_back(getU(t, yPre[t + xMax], 0));
                } else {
                    y.push_back(getU(t, yPre[t + xMax], yPre[t + xMax - 1]));
                }
            } else if (what == 1) {
                if (t == -xMax) {
                    y.push_back(getUCenter(t, yPre[t + xMax], 0, yPre[t + xMax + 1]));
                } else if (t == 2 * xMax) {
                    y.push_back(getUCenter(t, yPre[t + xMax], yPre[t + xMax - 1], 0));
                } else {
                    y.push_back(getUCenter(t, yPre[t + xMax], yPre[t + xMax - 1], yPre[t + xMax + 1]));
                }
            } else if (what == 2) {
                if (t == -xMax) {
                    y.push_back(getUSquare(t, yPre[t + xMax], 0, 0));
                } else {
                    y.push_back(getUSquare(t, yPre[t + xMax], yPre[t + xMax - 1], y[t + xMax - 1]));
                }
                /*
                if (t == -xMax) {
                    y2.push_back(getUCenter(t, yPre2[t + xMax], 0, yPre2[t + xMax + 1]));
                } else if (t == 2 * xMax) {
                    y2.push_back(getUCenter(t, yPre2[t + xMax], yPre2[t + xMax - 1], 0));
                } else {
                    y2.push_back(getUCenter(t, yPre2[t + xMax], yPre2[t + xMax - 1], yPre2[t + xMax + 1]));
                }
                */
                if (t == -xMax) {
                    y2.push_back(getU(t, yPre2[t + xMax], 0));
                } else {
                    y2.push_back(getU(t, yPre2[t + xMax], yPre2[t + xMax - 1]));
                }
            } else if (what == 3) {
                if (t == -xMax) {
                    y.push_back(getUHopf(t, yPre[t + xMax], 0, 0));
                } else {
                    y.push_back(getUHopf(t, yPre[t + xMax], yPre[t + xMax - 1], y[t + xMax - 1]));
                }
            }
    //        if (t < xMax + 5)
    //            qDebug() << x[t + xMax] << y[y.size() - 1];
        }
    }
    stepNumber++;

    ui->graph->clearGraphs();//Если нужно, но очищаем все графики
    double minY = y[0], maxY = y[0];
    for (int i = 1; i < y.size(); i++) {
        if (y[i] < minY)
            minY = y[i];
        if (y[i] > maxY)
            maxY = y[i];
    }
    for (int i = 0; i < y2.size(); i++) {
        if (y2[i] < minY)
            minY = y2[i];
        if (y2[i] > maxY)
            maxY = y2[i];
    }
    ui->graph->addGraph();
    ui->graph->graph(0)->setData(x, y);
    ui->graph->addGraph();
    ui->graph->graph(1)->setPen(QColor(255, 0, 0));
    ui->graph->graph(1)->setData(x, y2);

    ui->graph->xAxis->setRange(-xMax, xMax + xMax);//Для оси Ox
    ui->graph->xAxis->setLabel(QString::fromUtf8("x"));
    ui->graph->yAxis->setLabel(QString::fromUtf8("y"));
/*    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (int i = 0; i < year; i++) {
        textTicker->addTick(i, QString::number(i));
    }
    ui->graph->xAxis->setTicker(textTicker);*/
    ui->graph->yAxis->setRange(minY, maxY);//Для оси Oy
    ui->graph->replot();
}
