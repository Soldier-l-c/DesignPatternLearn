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
#include <map>

#include "Logger.h"
#include "DesignBase.h"
#include "noncopyable.h"

#ifdef _WIN32
#include <Windows.h>
#endif // WIN32_

#ifndef INFO
#define INFO 30
#endif // !INFO

#ifndef ERROR
#define ERROR 25
#endif // !ERROR

#ifndef DEBUG
#define DEBUG 20
#endif // !DEBUG

#define LOG(level) Logger(level)
#define MILL_SECOND(n) std::chrono::milliseconds(n)

#endif // !STDAFX_H
