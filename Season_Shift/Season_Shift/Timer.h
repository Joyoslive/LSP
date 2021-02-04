#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <chrono>

// Calculates deltaTime for every frame! --> No averaging done here.


class Timer
{

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
    std::chrono::microseconds m_duration;

public:
    enum class Duration
    {
        SECONDS,
        MILLISECONDS,
        MICROSECONDS
    };

    Timer();
    ~Timer();

    void start();
    void stop();

    // Print duration in milliseconds and seconds
    void print(std::wstring headerMsg);

    // Get duration: MILLISECONDS, SECONDS
    long double getTime(Duration duration);

    // Shorthand for get duration seconds
    long double dt();
};