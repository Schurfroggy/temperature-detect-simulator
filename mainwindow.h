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
    explicit MainWindow(QTimer *timer, QWidget *parent = nullptr);//�µĹ��캯������Ҫ����һ��QTimeָ�룬������Ϊ��mainwindow��������Ҫ����Qtime������
    ~MainWindow();
    QTimer* getTimer();

public slots:
    void tickFlush();//�൱��QTimer�����ʱ�����ʱ�����ĺ���

    void on_cycleDoubleSpinBox_valueChanged(double arg1);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_lowDoubleSpinBox_valueChanged(double arg1);

    void on_highDoubleSpinBox_valueChanged(double arg1);

    void on_wowButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;//��ʱ��
    QChart *chart;//����ͼ�������丽������
    QValueAxis* axisX;
    QValueAxis* axisY;
    QSplineSeries* line_series;
    bool wowFlag;//���ڼ�ʱ����ͣ
};
#endif // MAINWINDOW_H
