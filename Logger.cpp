#include "stdafx.h"
#include "Logger.h"

std::string GetFormatTime()
{
	time_t currentTime;
	time(&currentTime);
	tm t_tm;
	localtime_s(&t_tm, &currentTime);

	char formatTime[64] = { 0 };
	snprintf(formatTime, 64, "%02d:%02d:%02d",
		t_tm.tm_hour,
		t_tm.tm_min,
		t_tm.tm_sec);

	return std::string(formatTime);
}

Logger::Logger()
{
	std::cout << GetFormatTime();

	std::cout.width(8);
	std::cout << std::this_thread::get_id();

	std::cout.width(4);
	std::cout << "";
}

Logger::Logger(int32_t level) : Logger()
{
	this->level = level;
	std::cout.width(4);
	std::cout << this->level;

	std::cout.width(4);
	std::cout << "";
}

Logger::~Logger()
{
	std::cout << std::endl;
}
;
