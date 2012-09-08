#include "Logger.h"

using namespace std;
using namespace Log;

Logger::Logger() {
	_basicLogger = new BasicLogger(NULL);
	cout.setf(std::ios::fixed, std::ios::floatfield);
	cout.setf(std::ios::showpoint);
}

Logger& Logger::instance() {
	static Logger * logger_instance = new Logger();
	return *logger_instance;
}

Logger::~Logger() {
	*this << Log::info << "Log is closed";
	closeLog();
}

void Logger::openLog(const char *filename) {
	if(_log.is_open()) closeLog();
	_log.open(filename, ios::out);
	_log.setf(std::ios::fixed, std::ios::floatfield);
	_log.setf(std::ios::showpoint);
	_basicLogger = new BasicLogger(&_log);
}

void Logger::closeLog() {
	if(_log.is_open()) {
		_log << endl << endl;
		_log.close();
	}
	std::cout << endl << endl;
	delete _basicLogger;
}

void Logger::flush() {
	_log.flush();
}

const char * Logger::stringLogLevel(const LOG_LEVEL& level) const {
	switch(level) {
	case fatal: return "[FATAL] ";
	case error: return "[ERROR] ";
	case warning: return "[ WARN] ";
	case info: return "[ INFO] ";
	case debug: return "[DEBUG] ";
	}
	return "";
}

// Basic Logger

BasicLogger::BasicLogger() {

}

BasicLogger::BasicLogger(const ofstream *log) {
	_log = const_cast<ofstream*>(log);
}

BasicLogger::~BasicLogger() {
}

