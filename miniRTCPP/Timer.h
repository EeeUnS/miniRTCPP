#pragma once

#include <chrono>
#include <string>
#include <vector>
//std::chrono::duration<long long, std::nano>
class Timer
{
	static std::vector< std::chrono::steady_clock::time_point> chrono;
public:
	static void start();
	static void end(const std::string &Information);
};
