#pragma once
#include "TempInfo.h"

class TemperSensor {
public:
	TemperSensor();
	void newTemper();//���µ�ǰ��⵽���¶ȵ���Ϣ
	TempInfo getTemperInfo();
private:
	TempInfo temp_info;
};