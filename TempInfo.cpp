#include "TempInfo.h"
using namespace std;

TempInfo::TempInfo() {
	///nothing to do
}


string TempInfo::toString() {
	string temp_info_string;
	stringstream temper_str;
	temper_str << fixed << setprecision(1);
	temper_str << this->temper;

	struct tm timeinfo;
	struct tm _time;
	localtime_s(&_time,&this->temp_time);
	stringstream temp_time_str;
	temp_time_str << _time.tm_year + 1900 << "/";
	temp_time_str << _time.tm_mon + 1 << "/";
	temp_time_str << _time.tm_mday << "/ ";
	temp_time_str << _time.tm_hour << ":";
	temp_time_str << _time.tm_min << ":";
	temp_time_str << _time.tm_sec;
	temp_info_string ="temperature: "+ temper_str.str() + " " + temp_time_str.str() + " " + this->warning_msg;
	return temp_info_string;
}

