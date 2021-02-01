#include "Logger.h"
#include <chrono>
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
}

void Logger::debugLog(std::string log)
{
#ifdef UNICODE
	size_t reqSize = MultiByteToWideChar(CP_UTF8, 0, log.c_str(), -1, nullptr, 0);
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
