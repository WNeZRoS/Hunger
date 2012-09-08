#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "compatibility.h"

namespace Log {

enum LOG_LEVEL
{
	fatal,
	error,
	warning,
	info,
	debug
};

// Classes in this file have realization of some method in header for normally workds template in GCC

#define FILE "(" << __FILE__ << ":" << __LINE__ << ")  "

class BasicLogger
{
public:
	BasicLogger(const std::ofstream *log);
	~BasicLogger();

	template <typename T>
	BasicLogger& operator <<(const T& data) {
		if(_log && _log->is_open()) *_log << data;
		std::cout << data;
		std::cout.flush();
		return *this;
	}
protected:
	BasicLogger();
private:
	std::ofstream *_log;
};

class Logger : public BasicLogger
{
public:
	static Logger& instance();

	void openLog(const char *filename);
	void closeLog();
	void flush();

	template <typename T>
	BasicLogger& operator <<(const T& data) {
		return *this;
	}

	BasicLogger& operator << (const LOG_LEVEL& type) {
#ifndef DEBUG
		if(type == debug) {
			return *this;
		}
#endif

		using namespace std;

		Time time;
		getTime(time);

		stringstream str;
		str << endl << stringLogLevel(type) << setfill('0') << setw(2) << time.hour << ":"
						 << setw(2) << time.minute << ":" << setw(2) << time.second << "."
						 << setw(3) << time.milliseconds << "   ";
		if(!_log.is_open()) _log << str.str();
		cout << str.str();
		return *_basicLogger;
	}
protected:
	Logger();
	~Logger(); // Never called

private:
	BasicLogger *_basicLogger;
	std::ofstream _log;

	const char * stringLogLevel(const LOG_LEVEL& level) const;
};


#define logger Logger::instance()
#define Debug logger << Log::debug
};

#endif // LOGGER_H
