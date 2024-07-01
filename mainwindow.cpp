#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QTimer*_timer,QWidget *parent)
    : timer(_timer)
    , QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    wowFlag=0;
    int interval=TempSys::getInstance()->getCycle()*1000;
    timer->setInterval(interval);
    connect(timer, &QTimer::timeout, this, &MainWindow::tickFlush);
    //设置表格格式
    ui->setupUi(this);
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);

    QFont font;
    font.setPointSize(14);
    quint16 temperSymbol[]={0xe78e,0x0043,0};
    ui->cycleDoubleSpinBox->setSuffix(QString::fromLocal8Bit(" 秒/次"));
    ui->cycleDoubleSpinBox->setFont(font);
    ui->lowDoubleSpinBox->setSuffix(" "+QString::fromUtf16(temperSymbol));
    ui->lowDoubleSpinBox->setFont(font);
    ui->highDoubleSpinBox->setSuffix(" "+QString::fromUtf16(temperSymbol));
    ui->highDoubleSpinBox->setFont(font);

    ui->alarmLabel_1->hide();
    ui->alarmLabel_2->hide();
    ui->highAlarmLabel->hide();
    ui->lowAlarmLabel->hide();
    //设置曲线图格式
    axisX=new QValueAxis();
    axisY=new QValueAxis();
    axisX->setTitleText(QString::fromLocal8Bit("记录"));
    axisY->setTitleText(QString::fromLocal8Bit("温度/")+QString::fromUtf16(temperSymbol));
    axisX->setMin(0);
    axisX->setMax(20);
    axisY->setMin(-10.0);
    axisY->setMax(60.0);
    line_series=new QSplineSeries();
    line_series->setPointsVisible(true);
    line_series->setName(QString::fromLocal8Bit("温度曲线"));

    chart=new QChart();
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->addSeries(line_series);

    line_series->attachAxis(axisX);
    line_series->attachAxis(axisY);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setMinimumHeight(350);
    ui->chartView->setMinimumWidth(530);
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}

QTimer* MainWindow::getTimer(){
    return timer;
}

void MainWindow::tickFlush(){
    TempSys::getInstance()->tickTempSys();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setColumnCount(1);
    quint16 temperSymbol[]={0xe78e,0x0043,0};
    QTableWidgetItem *instantTemp=new QTableWidgetItem(QString::number(TempSys::getInstance()->getInstantTemp(),10,1)+QString::fromUtf16(temperSymbol));
    QTableWidgetItem *averageTemp=new QTableWidgetItem(QString::number(TempSys::getInstance()->getAverateTemp(),10,1)+QString::fromUtf16(temperSymbol));
    QTableWidgetItem *highestTemp=new QTableWidgetItem(QString::number(TempSys::getInstance()->getHighestTemp(),10,1)+QString::fromUtf16(temperSymbol));
    QTableWidgetItem *lowestTemp=new QTableWidgetItem(QString::number(TempSys::getInstance()->getLowestTemp(),10,1)+QString::fromUtf16(temperSymbol));
    QTableWidgetItem *readingCycle=new QTableWidgetItem(QString::number(TempSys::getInstance()->getCycle(),10,1)+QString::fromLocal8Bit(" 秒/次"));
    //更新表格信息
    instantTemp->setTextAlignment(Qt::AlignVCenter);
    averageTemp->setTextAlignment(Qt::AlignVCenter);
    highestTemp->setTextAlignment(Qt::AlignVCenter);
    lowestTemp->setTextAlignment(Qt::AlignVCenter);
    readingCycle->setTextAlignment(Qt::AlignVCenter);

    highestTemp->setTextColor(QColor(255,0,4));
    lowestTemp->setTextColor(QColor(0,255,255));

    ui->tableWidget->setItem(0,0,instantTemp);
    ui->tableWidget->setItem(1,0,averageTemp);
    ui->tableWidget->setItem(2,0,highestTemp);
    ui->tableWidget->setItem(3,0,lowestTemp);
    ui->tableWidget->setItem(4,0,readingCycle);
    //更新曲线图内容
    int count=line_series->points().size();
    if(count>TempSys::getInstance()->getWindowSize()){
        chart->axisX()->setMax(count);
        chart->axisX()->setMin(count-20);
    }
    line_series->setPointsVisible(true);
    line_series->append(QPointF(count,QString::number(TempSys::getInstance()->getInstantTemp(),10,1).toDouble()));

    if(TempSys::getInstance()->getInstantAlarmSituation()==-1){
        ui->highAlarmLabel->hide();
        ui->lowAlarmLabel->show();
    }
    else if(TempSys::getInstance()->getInstantAlarmSituation()==1){
        ui->highAlarmLabel->show();
        ui->lowAlarmLabel->hide();
    }
    else{
        ui->highAlarmLabel->hide();
        ui->lowAlarmLabel->hide();
    }

}

void MainWindow::on_cycleDoubleSpinBox_valueChanged(double arg1)
{
    TempSys::getInstance()->setCycle(arg1);
    this->timer->setInterval(TempSys::getInstance()->getCycle()*1000);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    TempSys::getInstance()->switchFilterAlgorithm(index);
}

void MainWindow::on_lowDoubleSpinBox_valueChanged(double arg1)
{
    bool res = TempSys::getInstance()->setLowestAlarmTemp(arg1);
    if(res==0)
        ui->alarmLabel_1->show();
    else
        ui->alarmLabel_1->hide();
}

void MainWindow::on_highDoubleSpinBox_valueChanged(double arg1)
{
    bool res = TempSys::getInstance()->setHighestAlarmTemp(arg1);
    if(res==0)
        ui->alarmLabel_2->show();
    else
        ui->alarmLabel_2->hide();
}

void MainWindow::on_wowButton_clicked()
{
    if(wowFlag==0){
        this->timer->stop();
        ui->wowButton->setText(QString::fromLocal8Bit("继续"));
        wowFlag=1;
    }
    else{
        this->timer->start();
        ui->wowButton->setText(QString::fromLocal8Bit("暂停"));
        wowFlag=0;
    }
}

