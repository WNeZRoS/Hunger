#include "Thread.h"
#include <iostream>
#include <stdexcept>

Thread::Thread() {
	_running = false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&_thread, &attr, Thread::ThreadFunc, this);
	pthread_attr_destroy(&attr);
}

Thread::~Thread() {
	_running = false;
}

void Thread::addEvent(int command, void *param) {
	Event event = { command, param };

	_eventMutex.lock();
	_events.push_back(event);
	_eventMutex.unlock();
}

bool Thread::isRunning() const {
	return _running;
}

bool Thread::run() {
	return false;
}

void Thread::onEvent(int command, void *param) {

}

void Thread::onStart() {

}

void Thread::onExit() {

}

void Thread::stop() {
	_running = false;
}

void Thread::wait() {
	pthread_join(_thread, NULL);;
}

void * Thread::ThreadFunc(void *arg) {
	Thread *thread = static_cast<Thread *>(arg);
	thread->_running = true;

	thread->onStart();

	while(thread->_running) {

		for(std::list<Event>::iterator it = thread->_events.begin(); it != thread->_events.end(); it++) {
			thread->onEvent((*it).command, (*it).param);
		}

		thread->_eventMutex.lock();
		thread->_events.clear();
		thread->_eventMutex.unlock();

		if(!thread->run()) thread->stop();
	}

	thread->onExit();

	pthread_exit(NULL);
	return 0;
}

// Mutex

#ifdef lock
#undef lock
#undef onelock
#undef trylock
#undef LocalMutex
#endif

Mutex::Mutex() {
	pthread_mutex_init(&_mutex, NULL);
	_locked = false;
	_thread = pthread_self();
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&_mutex);
}

#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
void Mutex::_lock(const char *file, int line) {
	if(_locked && pthread_equal(_thread, pthread_self()) != 0) {
		std::cout << "Thread already lock mutex at " << _file << ":" << _line << "." << std::endl;
		std::cout.flush();
	}

	pthread_mutex_lock(&_mutex);
	_locked = true;
	_thread = pthread_self();

	_file = file;
	_line = line;
}

bool Mutex::_onelock(const char *file, int line) {
	if(_locked && pthread_equal(_thread, pthread_self()) != 0) return false;
	this->_lock(file, line);
	return true;
}

bool Mutex::_trylock(const char *file, int line) {
	if(0 != pthread_mutex_trylock(&_mutex)) {
		_locked = true;
		_thread = pthread_self();

		_file = file;
		_line = line;
		return true;
	}
	return false;
}

#else
void Mutex::lock() {
	if(_locked && pthread_equal(_thread, pthread_self()) != 0) {
		std::cout << "Thread already lock mutex." << std::endl;
		std::cout.flush();
	}

	pthread_mutex_lock(&_mutex);

	_locked = true;
	_thread = pthread_self();
}

bool Mutex::onelock() {
	if(_locked && pthread_equal(_thread, pthread_self()) != 0) return false;
	this->lock();
	return true;
}

bool Mutex::trylock() {
	return 0 != pthread_mutex_trylock(&_mutex);
}

#endif

void Mutex::unlock() {
	_locked = false;
	pthread_mutex_unlock(&_mutex);
}


// LocalMutex

#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
FuncMutex::FuncMutex(Mutex &mutex, const char *file, int line) {
	_mutex = &mutex;
	_mutex->_lock(file, line);
}
#else
FuncMutex::FuncMutex(Mutex &mutex) {
	_mutex = &mutex;
	_mutex->lock();
}
#endif

FuncMutex::~FuncMutex() {
	_mutex->unlock();
}

// Cond

Cond::Cond() {
	pthread_cond_init(&_cond, NULL);
}

Cond::~Cond() {
	pthread_cond_destroy(&_cond);
}

void Cond::wait(Mutex& mutex) {
	pthread_cond_wait(&_cond, &mutex._mutex);
}

void Cond::signal() {
	pthread_cond_signal(&_cond);
}
