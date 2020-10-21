#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double tau, u0, h, v, tMax, pi = 3.14, stepNumber = 0;
    QVector<double> x, y, y2; //Массивы координат точек

    double getU(double t, double yPre, double yPrePre);
    double getUCenter(double t, double yPre, double yPrePre, double yNext);
    double getUSquare(double t, double yPre, double yPrePre, double yThis);
    double getUHopf(double t, double yPre, double yPrePre, double yNext);
    void draw();

public slots:
    void generate();
    void step();
};

#endif // MAINWINDOW_H
