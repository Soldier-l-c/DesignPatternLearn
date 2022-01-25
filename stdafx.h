#pragma once
#ifndef STDAFX_H
#define STDAFX_H
#include <iostream>
#include <vector>
#include < algorithm >
#include <string>
#include <thread>
#include <chrono>

#define LOG std::cout
#define END std::endl

#define MILL_SECOND(n) std::chrono::milliseconds(n)
#endif // !STDAFX_H
