#pragma once

#include <string>
#include <vector>

class Logger
{
private:
	struct Log
	{
		std::string message;
		std::string timeStamp;
		std::string filePath;
	};
private:
	static Logger* instance;
	std::vector<Log> m_bufferedLogs;
	
private:
	Logger();
public:
	~Logger();

	static Logger& getLogger();
	void addLog(std::string log, std::string filePath);
	void debugLog(std::string message);
	void dumpLogs();
};

