#include "pch.h"
#include "Timer.h"

using namespace std;
using namespace chrono;

Timer::Timer() :
    m_start(high_resolution_clock::now()),
    m_end(high_resolution_clock::now()),
    m_duration(0)
{

}

Timer::~Timer()
{

}

void Timer::start()
{
    m_start = high_resolution_clock::now();
}

void Timer::stop()
{
    m_end = high_resolution_clock::now();
    m_duration = duration_cast<microseconds>(m_end - m_start);
}

void Timer::print(std::wstring headerMsg)
{
    // Printing ms with IE-03 to keep the decimals from microseconds
    // Printing s with a duration cast first to remove three decimals (only purpose for s is to have a reference point to a second)
    //auto durationInMs = duration_.count() * 1E-03;
    //auto durationInS = duration_cast<milliseconds>(duration_).count() * 1E-03;

    //cout << "\n" << "--- " << headerMsg << " ---" << endl
    //    << "Elapsed: " << durationInMs << "ms "
    //    << "(" << durationInS * 1E-03 << "s)." << endl;

    std::wstring outputStr(
        L"\n--- " + headerMsg + L" ---\n" +
        L"Elapsed: " + std::to_wstring(getTime(Duration::MILLISECONDS)) + L"ms " +
        L"(" + std::to_wstring(getTime(Duration::SECONDS)) + L"s).\n"
    );
    OutputDebugStringW(outputStr.c_str());
}

long double Timer::getTime(Duration duration)
{
    switch (duration)
    {
    case Duration::MILLISECONDS:
        return m_duration.count() * 1E-03;    // Milliseconds with decimal
        break;
    case Duration::SECONDS:
        return m_duration.count() * 1E-06;    // Seconds with decimal
        break;
    case Duration::MICROSECONDS:
        return static_cast<long double>(m_duration.count());
        break;

        // Microseconds default
    default:
        return static_cast<long double>(m_duration.count());
    }
}

long double Timer::dt()
{
    return m_duration.count() * 1E-06;    // Seconds with decimal
}