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

}

void LogFile::onExit() {
	delete _log;
	delete _dataMutex;
}

bool LogFile::run() {
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
	return true;
}

Logger::Logger() {
	_logMutex = new Mutex;
	_log = NULL;
	_basicLogger = NULL;
	openLog(NULL);
}

Logger& Logger::instance() {
	return *instance_pointer();
}

Logger * Logger::instance_pointer() {
	static Logger * logger_instance = new Logger();
	return logger_instance;
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
	if(_log) {
		_log->stop();
		_log->wait();
		delete _log;
	}
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

