#pragma once

#include <chrono>
#include <string>
#include <vector>
//std::chrono::duration<long long, std::nano>
namespace Timer
{
	void start();
	void end(const std::string &Information);
};
