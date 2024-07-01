#pragma once
#include "TemperSensor.h"
#include<fstream>
using namespace std;

class TempSys {
public:
    static TempSys* getInstance();//单例模式，需要获取当前实例

    double calculateAverageTemp();//计算平均温度，这里的平均温度是所有温度数据的平均而不仅限于window_size的数据
    void tickTempSys();//每次刷新数据时监控系统的操作，包括更新两个数据存储数组，刷新平均、最高、最低温度，更新日志等
    TempInfo movingAverageFilterAlgorithm();//滤波算法，移动平均滤波
    TempInfo exponentiallyWeightedMovingAverageAlgorithm();//滤波算法，指数加权移动平均滤波
    void switchFilterAlgorithm(bool alg);//更改当前使用的算法
    void temperLimitWarningMsg(TempInfo& temp_info);//刷新警告信息，这一操作执行在滤波算法被执行后

    /*getters*/
    double getInstantTemp();
    time_t getInstantTime();
    TempInfo getInstantTempInfo();
    TempInfo* getTemperQueue();
    TempInfo* getFilteredTemperQueue();
    double getAverateTemp();
    double getHighestTemp();
    double getLowestTemp();
    double getHighestAlarmTemp();
    double getLowestAlarmTemp();
    double getCycle();
    int getWindowSize();
    int getInstantAlarmSituation();

    /*setters*/
    bool setLowestAlarmTemp(double l_a_t);
    bool setHighestAlarmTemp(double h_a_t);
    bool setAlarmTemp(double l_a_t, double h_a_t);
    bool setCycle(double cycle);

    string toString();//用于向日志输出温度信息
private:
    TempSys();
    ~TempSys();
    TempSys(const TempSys&) = default;
    TempSys& operator=(const TempSys&) = default;

    TemperSensor temper_sensor;//温度模拟源
    TempInfo* temper_queue;//原始数据指针，用于存放原始数据
    TempInfo* filtered_temper_queue;//存储滤波后的温度数据
    int filled_data;//表示已经拥有的温度数据数量

    double avg_temper;//平均温度
    double highest_temper;//最高温度
    double lowest_temper;//最低温度

    double lowest_alarm_temp;//最低温度警报限制
    double highest_alarm_temp;//最高温度警报限制
    double cycle;//查询周期

    static const int window_size = 20;//窗口大小，用于移动平均滤波计算和前端显示约束

    bool filter_data_alogorithm;//默认滤波算法，0表示移动平均滤波

    const string HIGH_TEMPER_WARNING = "WARNING:TEMPERATURE HIGHER THAN LIMIT";
    const string LOW_TEMPER_WARNING = "WARNING:TEMPERATURE LOWER THAN LIMIT";

    fstream write_log;//日志文件
};
