#pragma once
#ifndef STDAFX_H
#define STDAFX_H
#include <iostream>
#include <vector>
#include < algorithm >
#include <string>
#include <thread>
#include <chrono>

#include <sstream>

class Logger 
{
public:
	Logger() = default;
	~Logger()
	{
		std::cout << std::endl;
	}

	template <class T>
	Logger& operator<<(const T& t)
	{
		std::cout << t;
		return *this;
	}
};

#define LOG Logger()
#define MILL_SECOND(n) std::chrono::milliseconds(n)

#endif // !STDAFX_H
