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
	time_t temp_time;//��¼�¶�ʱ��
	std::string warning_msg;//������Ϣ
	std::string toString();//���ڴ�ӡ�¶���Ϣ
};