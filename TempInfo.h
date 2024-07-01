#pragma once
#include<string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

class TempInfo
{
public:
	TempInfo();
	double temper;
	time_t temp_time;//记录温度时间
	std::string warning_msg;//报错信息
	std::string toString();//用于打印温度信息
};