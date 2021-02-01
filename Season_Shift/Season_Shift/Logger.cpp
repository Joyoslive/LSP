#include "Logger.h"
#include <chrono>
#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>
#include <Windows.h>

Logger* Logger::instance = nullptr;

Logger::Logger() {}

Logger::~Logger() 
{
	Log destructorLog = {};
	m_bufferedLogs.emplace(m_bufferedLogs.begin(), destructorLog);
	dumpLogs();
}

Logger& Logger::getLogger()
{
	if (!instance)
	{
		instance = new Logger();
	}

	return *instance;
}

void Logger::addLog(std::string log, std::string filePath)
{
	auto timeStamp = std::chrono::system_clock::now();
	time_t timetTime = std::chrono::system_clock::to_time_t(timeStamp);
	std::stringstream ss;
	tm date;
	localtime_s(&date, &timetTime);
	ss << std::put_time(&date, "%F %T");
	Log newLog = {
		log, ss.str(), filePath
	};
	m_bufferedLogs.emplace_back(newLog);
}

void Logger::debugLog(std::string log)
{
#ifdef UNICODE
	auto reqSize = MultiByteToWideChar(CP_UTF8, 0, log.c_str(), -1, nullptr, 0);
	auto wLog = new wchar_t[reqSize];
	MultiByteToWideChar(CP_UTF8, 0, log.c_str(), -1, wLog, reqSize);
	OutputDebugStringW(wLog);
	delete[] wLog;
#else
	OutputDebugStringA(log.c_str());
#endif
}

void Logger::dumpLogs()
{

}
