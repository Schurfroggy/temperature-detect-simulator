#pragma once
#include "TempInfo.h"

class TemperSensor {
public:
	TemperSensor();
	void newTemper();//更新当前检测到的温度的信息
	TempInfo getTemperInfo();
private:
	TempInfo temp_info;
};