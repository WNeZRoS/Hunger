#include "Logger.h"

using namespace std;
using namespace Log;

LogFile::LogFile(const char *filename) {
	_dataMutex = new Mutex;
	if(filename) _log = FileManager::instance().write(filename);
	else _log = NULL;
	_str.str( string() );
	_str.setf(std::ios::fixed, std::ios::floatfield);
	_str.setf(std::ios::showpoint);
}

LogFile::~LogFile() {
	delete _log;
	delete _dataMutex;
}

Logger::Logger() {
	_logMutex = new Mutex;
	_log = NULL;
	_basicLogger = NULL;
	openLog(NULL);
}

Logger& Logger::instance() {
	static Logger * logger_instance = new Logger();
	return *logger_instance;
}

Logger::~Logger() {
	*this << Log::info << "Log is closed";
	closeLog();
	delete _logMutex;
}

void Logger::openLog(const char *filename) {
	if(_log) closeLog();
	_log = new LogFile(filename);
	_basicLogger = new BasicLogger(_log);
}

void Logger::closeLog() {
	if(_log) delete _log;
	delete _basicLogger;
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
	_log = NULL;
}

BasicLogger::BasicLogger(const LogFile *log) {
	_log = const_cast<LogFile*>(log);
}

BasicLogger::~BasicLogger() {
}

