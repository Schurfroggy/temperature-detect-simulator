#include "TempSys.h"
#include<iostream>

using namespace std;

TempSys* TempSys::getInstance() {
    static TempSys instance;
    return &instance;
}

TempSys::TempSys() {
    temper_queue = new TempInfo[500];
    filtered_temper_queue = new TempInfo[500];//限制数据上线为500
    filled_data = 1;

    lowest_alarm_temp = 0;
    highest_alarm_temp = 35;

    temper_queue[filled_data - 1] = temper_sensor.getTemperInfo();
    filtered_temper_queue[filled_data - 1] = temper_sensor.getTemperInfo();
    temperLimitWarningMsg(filtered_temper_queue[filled_data - 1]);

    avg_temper = filtered_temper_queue[filled_data - 1].temper;
    highest_temper = filtered_temper_queue[filled_data - 1].temper;
    lowest_temper = filtered_temper_queue[filled_data - 1].temper;

    cycle = 1;

    filter_data_alogorithm = 0;

    write_log.open("record_log.txt", ios::out | ios::app);
    write_log << toString() << endl;
    write_log.close();
}

TempSys::~TempSys() {
    write_log.open("record_log.txt", ios::out | ios::app);
    write_log << "--------------------------------------------------------------" << endl;
    write_log.close();
}

double TempSys::calculateAverageTemp() {
    double sum = 0;
    for (int i = 0; i < filled_data; i++) {
        sum += filtered_temper_queue[i].temper;
    }
    this->avg_temper = (double)sum / filled_data;
    return avg_temper;
}

TempInfo TempSys::movingAverageFilterAlgorithm() {
    TempInfo temp_info;
    double sum = 0;
    int data_amount = (filled_data > window_size) ? window_size : filled_data;
    for (int i = filled_data - 1; i >= filled_data - data_amount; i--) {
        sum += this->temper_queue[i].temper;
    }
    temp_info.temper = (double)sum / data_amount;
    temp_info.temp_time = temper_queue[filled_data - 1].temp_time;
    return temp_info;
}

TempInfo TempSys::exponentiallyWeightedMovingAverageAlgorithm() {
    TempInfo temp_info;
    if (this->filled_data == 1) {
        temp_info.temper = this->temper_queue[filled_data - 1].temper;
        temp_info.temp_time = this->temper_queue[filled_data - 1].temp_time;
        return temp_info;
    }
    const double alpha = 0.2;
    temp_info.temper = alpha * this->temper_queue[filled_data - 1].temper + (1 - alpha)*this->filtered_temper_queue[filled_data-2].temper;
    temp_info.temp_time = this->temper_queue[filled_data - 1].temp_time;
    return temp_info;
}

void TempSys::temperLimitWarningMsg(TempInfo& temp_info) {
    if (temp_info.temper < lowest_alarm_temp)
        temp_info.warning_msg = LOW_TEMPER_WARNING;
    else if (temp_info.temper > highest_alarm_temp)
        temp_info.warning_msg = HIGH_TEMPER_WARNING;
    else temp_info.warning_msg = "";
}

int TempSys::getInstantAlarmSituation(){
    if (this->filtered_temper_queue[filled_data-1].temper<lowest_alarm_temp)
        return -1;
    else if (this->filtered_temper_queue[filled_data-1].temper > highest_alarm_temp)
        return 1;
    else return 0;
}

void TempSys::tickTempSys() {
    //1 cycle action
    temper_sensor.newTemper();
    filled_data++;
    temper_queue[filled_data - 1] = temper_sensor.getTemperInfo();

    TempInfo temp_info;
    if (this->filter_data_alogorithm == 0)
        temp_info = movingAverageFilterAlgorithm();
    else temp_info = exponentiallyWeightedMovingAverageAlgorithm();
    temperLimitWarningMsg(temp_info);
    cout << temp_info.warning_msg << endl;
    filtered_temper_queue[filled_data - 1] = temp_info;

    calculateAverageTemp();
    if (temp_info.temper < lowest_temper)
        this->lowest_temper = temp_info.temper;
    if (temp_info.temper > highest_temper)
        this->highest_temper = temp_info.temper;

    write_log.open("record_log.txt", ios::out | ios::app);
    write_log << toString() << endl;
    write_log.close();
}

void TempSys::switchFilterAlgorithm(bool alg) {
    this->filter_data_alogorithm = alg;
}

/*getters*/
double TempSys::getInstantTemp() {
    return this->filtered_temper_queue[filled_data - 1].temper;
}

time_t TempSys::getInstantTime() {
    return this->filtered_temper_queue[filled_data - 1].temp_time;
}

TempInfo TempSys::getInstantTempInfo() {
    return this->filtered_temper_queue[filled_data - 1];
}

TempInfo* TempSys::getTemperQueue() {
    return this->temper_queue;
}

TempInfo* TempSys::getFilteredTemperQueue() {
    return this->filtered_temper_queue;
}

double TempSys::getAverateTemp() {
    return this->avg_temper;
}

double TempSys::getHighestTemp() {
    return this->highest_temper;
}

double TempSys::getLowestTemp() {
    return this->lowest_temper;
}

double TempSys::getHighestAlarmTemp() {
    return this->highest_alarm_temp;
}

double TempSys::getLowestAlarmTemp() {
    return this->lowest_alarm_temp;
}

double TempSys::getCycle() {
    return this->cycle;
}

int TempSys::getWindowSize(){
    return this->window_size;
}

/*setters*/
bool TempSys::setLowestAlarmTemp(double l_a_t) {
    if (l_a_t < highest_alarm_temp) {
        lowest_alarm_temp = l_a_t;
        return true;
    }
    return false;
}

bool TempSys::setHighestAlarmTemp(double h_a_t) {
    if (h_a_t > lowest_alarm_temp) {
        highest_alarm_temp = h_a_t;
        return true;
    }
    return false;
}

bool TempSys::setAlarmTemp(double l_a_t, double h_a_t) {
    if (l_a_t < h_a_t) {
        lowest_alarm_temp = l_a_t;
        highest_alarm_temp = h_a_t;
        return true;
    }
    return false;
}

bool TempSys::setCycle(double cycle) {
    if (cycle > 0) {
        this->cycle = cycle;
        return true;
    }
    return false;
}

string TempSys::toString() {
    string temp_string;
    stringstream avg_temp;
    stringstream high_temp;
    stringstream low_temp;
    avg_temp << fixed << setprecision(1);
    high_temp << fixed << setprecision(1);
    low_temp << fixed << setprecision(1);
    avg_temp << this->avg_temper;
    high_temp << this->highest_temper;
    low_temp << this->lowest_temper;

    temp_string.append("record:" + to_string(this->filled_data));
    temp_string.append(" "+this->getInstantTempInfo().toString());
    temp_string.append(" average:" + avg_temp.str());
    temp_string.append(" highest:" + high_temp.str());
    temp_string.append(" lowest:" + low_temp.str());
    if (this->filter_data_alogorithm == 0)
        temp_string.append(" filter_alogorithm:moving_average_filter");
    else temp_string.append(" filter_alogorithm:exponentially_weighted_moving_average");
    return temp_string;
}
