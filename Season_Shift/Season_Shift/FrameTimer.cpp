#include "pch.h"
#include "FrameTimer.h"


using namespace std;
using namespace chrono;

FrameTimer::InternalTimer FrameTimer::InternalTimer::s_timer;

void FrameTimer::frameStart()
{
    InternalTimer::s_timer.start();
}

void FrameTimer::frameStop()
{
    InternalTimer::s_timer.stop();
}

void FrameTimer::print(std::wstring headerMsg)
{
    std::wstring outputStr(
        L"\n--- " + headerMsg + L" ---\n" +
        L"Elapsed: " + std::to_wstring(getTime(Duration::MILLISECONDS)) + L"ms " +
        L"(" + std::to_wstring(getTime(Duration::SECONDS)) + L"s).\n"
    );
    OutputDebugStringW(outputStr.c_str());
}

long double FrameTimer::getTime(Duration duration)
{
    switch (duration)
    {
    case Duration::MILLISECONDS:
        return InternalTimer::s_timer.m_duration.count() * 1E-03;    // Milliseconds with decimal
        break;
    case Duration::SECONDS:
        return InternalTimer::s_timer.m_duration.count() * 1E-06;    // Seconds with decimal
        break;
    case Duration::MICROSECONDS:
        return InternalTimer::s_timer.m_duration.count();
        break;

        // Microseconds default
    default:
        return InternalTimer::s_timer.m_duration.count();
    }
}

long double FrameTimer::dt()
{
    return InternalTimer::s_timer.m_duration.count() * 1E-06;    // Seconds with decimal
}

FrameTimer::InternalTimer::InternalTimer() :
    m_start(high_resolution_clock::now()),
        m_end(high_resolution_clock::now()),
        m_duration(0)
{

}

void FrameTimer::InternalTimer::start()
{
    m_start = high_resolution_clock::now();
}

void FrameTimer::InternalTimer::stop()
{
    m_end = high_resolution_clock::now();
    m_duration = duration_cast<microseconds>(m_end - m_start);
}
