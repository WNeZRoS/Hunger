#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <iomanip>

#include "compatibility.h"
#include "FileManager.h"
#include "Thread.h"

#ifdef ANDROID_NDK
#include <android/log.h>

#define  LOG_TAG    "Hunger"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

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

class LogFile : public Thread
{
public:
	LogFile(const char *filename);
	~LogFile();

	template <typename T>
	LogFile& operator <<(const T& data) {
		_dataMutex->lock();
		_str << data;
		_dataMutex->unlock();
		return *this;
	}

private:
	std::ofstream *_log;
	std::stringstream _str;
	std::string _lastStr;
	Mutex *_dataMutex;

	void run() {
		while(isRunning()) {
			msSleep(1000);
			_dataMutex->lock();
			size_t ln = _str.str().find("\n");
			while(ln != std::string::npos) {
				std::string line = _str.str().substr(0, ln);
				if(line.size() > 0) {
					if(_log) if(_log->is_open()) *_log << line << std::endl;
					std::cout << line << std::endl;
#ifdef ANDROID_NDK
					LOGD(line.c_str());
#endif
				}
				line = _str.str().length() > ln ? _str.str().substr(ln + 1) : "";
				 _str.str( std::string() );
				 _str << line;
				 ln = _str.str().find("\n");
			}

			if(_lastStr == _str.str() && _lastStr.size() > 0) {
				_str << std::endl;
			}
			_lastStr = _str.str();
			_dataMutex->unlock();
		}
	}
};

class BasicLogger
{
public:
	BasicLogger(const LogFile *log);
	virtual ~BasicLogger();

	template <typename T>
	BasicLogger& operator <<(const T& data) {
		*_log << data;
		return *this;
	}
protected:
	BasicLogger();

	LogFile *_log;
};

class Logger : public BasicLogger
{
public:
	~Logger();

	static Logger& instance();
	static Logger* instance_pointer();

	void openLog(const char *filename);
	void closeLog();

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
		_logMutex->lock();
		Time time;
		getTime(time);

		(*_log) << '\n' << stringLogLevel(type) << std::setfill('0') << std::setw(2) << time.hour << ":"
			<< std::setw(2) << time.minute << ":" << std::setw(2) << time.second << "."
			<< std::setw(3) << time.milliseconds << "   ";
		_logMutex->unlock();
		return *_basicLogger;
	}
protected:
	Logger();

private:
	BasicLogger *_basicLogger;
	Mutex *_logMutex;

	const char * stringLogLevel(const LOG_LEVEL& level) const;
};


#define logger Logger::instance()
#define Debug logger << Log::debug
}

#endif // LOGGER_H
