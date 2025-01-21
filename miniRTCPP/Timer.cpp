
#include "Timer.h"
#include <iostream>
#include "ASSERT.h"

std::vector<std::chrono::steady_clock::time_point> chrono;

void Timer::start()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << "starting timer" << std::endl;
    chrono.push_back(start);
}

void Timer::end(const std::string& Information)
{
    ASSERT (chrono.size() > 0 );

    auto start = chrono.back();
    chrono.pop_back();
    auto end = std::chrono::steady_clock::now() - start;

    std::cout << Information << " takes " << std::chrono::duration_cast<std::chrono::milliseconds>(end).count()
        << " ms" << std::endl;
}
