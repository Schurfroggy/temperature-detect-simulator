#pragma once
#include "TemperSensor.h"
#include<fstream>
using namespace std;

class TempSys {
public:
    static TempSys* getInstance();//����ģʽ����Ҫ��ȡ��ǰʵ��

    double calculateAverageTemp();//����ƽ���¶ȣ������ƽ���¶��������¶����ݵ�ƽ������������window_size������
    void tickTempSys();//ÿ��ˢ������ʱ���ϵͳ�Ĳ��������������������ݴ洢���飬ˢ��ƽ������ߡ�����¶ȣ�������־��
    TempInfo movingAverageFilterAlgorithm();//�˲��㷨���ƶ�ƽ���˲�
    TempInfo exponentiallyWeightedMovingAverageAlgorithm();//�˲��㷨��ָ����Ȩ�ƶ�ƽ���˲�
    void switchFilterAlgorithm(bool alg);//���ĵ�ǰʹ�õ��㷨
    void temperLimitWarningMsg(TempInfo& temp_info);//ˢ�¾�����Ϣ����һ����ִ�����˲��㷨��ִ�к�

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

    string toString();//��������־����¶���Ϣ
private:
    TempSys();
    ~TempSys();
    TempSys(const TempSys&) = default;
    TempSys& operator=(const TempSys&) = default;

    TemperSensor temper_sensor;//�¶�ģ��Դ
    TempInfo* temper_queue;//ԭʼ����ָ�룬���ڴ��ԭʼ����
    TempInfo* filtered_temper_queue;//�洢�˲�����¶�����
    int filled_data;//��ʾ�Ѿ�ӵ�е��¶���������

    double avg_temper;//ƽ���¶�
    double highest_temper;//����¶�
    double lowest_temper;//����¶�

    double lowest_alarm_temp;//����¶Ⱦ�������
    double highest_alarm_temp;//����¶Ⱦ�������
    double cycle;//��ѯ����

    static const int window_size = 20;//���ڴ�С�������ƶ�ƽ���˲������ǰ����ʾԼ��

    bool filter_data_alogorithm;//Ĭ���˲��㷨��0��ʾ�ƶ�ƽ���˲�

    const string HIGH_TEMPER_WARNING = "WARNING:TEMPERATURE HIGHER THAN LIMIT";
    const string LOW_TEMPER_WARNING = "WARNING:TEMPERATURE LOWER THAN LIMIT";

    fstream write_log;//��־�ļ�
};
