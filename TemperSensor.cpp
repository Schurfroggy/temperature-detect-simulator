#include "TemperSensor.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
using namespace std;

void TemperSensor::newTemper() {
	this->temp_info.temper = double((rand() % 701) / 10) - 10;
	this->temp_info.temp_time = time(0);
	this->temp_info.warning_msg = "";
}

TemperSensor::TemperSensor() {
	newTemper();
}

TempInfo TemperSensor::getTemperInfo() {
	return this->temp_info;
}
