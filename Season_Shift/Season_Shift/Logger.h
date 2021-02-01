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
	};
private:
	static Logger* instance;
	std::vector<Log> m_bufferedLogs;
	std::string m_logFile;
	
private:
	Logger();
public:
	~Logger();

	static Logger& getLogger();
	void setFile(std::string filepath);
	void addLog(std::string log);
	void debugLog(std::string message);
	void dumpLogs();
};

