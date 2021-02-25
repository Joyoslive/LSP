#include "pch.h"
#include "Logger.h"
#include <chrono>
#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <stdexcept>

Logger Logger::instance = Logger();

Logger::Logger() 
{
	m_logFile = "logs.txt";
}

Logger::~Logger() 
{
	if (m_bufferedLogs.size() > 0)
		dumpLogs();
}

Logger& Logger::getLogger()
{
	return instance;
}

void Logger::setFile(std::string filepath)
{
	if (m_bufferedLogs.size() > 0)
		dumpLogs();

	m_logFile = filepath;
}

void Logger::addLog(std::string log)
{
	auto timeStamp = std::chrono::system_clock::now();
	time_t timetTime = std::chrono::system_clock::to_time_t(timeStamp);
	std::stringstream ss;
	tm date;
	localtime_s(&date, &timetTime);
	ss << std::put_time(&date, "%F %T");
	Log newLog = {
		log, ss.str(),
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
	if (m_bufferedLogs.size() > 0)
	{
		bool stored = std::find(m_storedFiles.begin(), m_storedFiles.end(), m_logFile) != m_storedFiles.end();
		std::ofstream file(m_logFile, stored ? std::ios::app : std::ios::out);
		if (!file)
		{
			debugLog("Error: The file specified for the logger does not exist! \nLogs are dumped to logs.txt");
			file.open("logs.txt");
		}
		for (auto& l : m_bufferedLogs)
		{
			if (l.timeStamp != "") 
			{ 
				file << l.timeStamp;
				file << " | "; 
			}
			file << l.message << "\n";
		}
		m_bufferedLogs.clear();
		file.close();
		if (!stored)
			m_storedFiles.emplace_back(m_logFile);
	}
}
