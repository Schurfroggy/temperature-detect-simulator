#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "TempSys.h"
#include <QMainWindow>
#include <QTimer>
#include <QChartView>
#include <QValueAxis>
#include <QSplineSeries>

QT_BEGIN_NAMESPACE
QT_CHARTS_USE_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QTimer *timer, QWidget *parent = nullptr);//新的构造函数，需要传入一个QTime指针，这是因为在mainwindow对象中需要更新Qtime的属性
    ~MainWindow();
    QTimer* getTimer();

public slots:
    void tickFlush();//相当于QTimer对象的时间结束时出发的函数

    void on_cycleDoubleSpinBox_valueChanged(double arg1);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_lowDoubleSpinBox_valueChanged(double arg1);

    void on_highDoubleSpinBox_valueChanged(double arg1);

    void on_wowButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;//计时器
    QChart *chart;//曲线图表对象和其附属对象
    QValueAxis* axisX;
    QValueAxis* axisY;
    QSplineSeries* line_series;
    bool wowFlag;//用于计时器暂停
};
#endif // MAINWINDOW_H
