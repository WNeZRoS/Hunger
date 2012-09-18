#include "Thread.h"

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

bool Thread::isRunning() const {
	return _running;
}

void Thread::run() {

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
	thread->run();
	thread->_running = false;

	pthread_exit(NULL);
	return 0;
}

// Mutex

Mutex::Mutex() {
	pthread_mutex_init(&_mutex, NULL);
}

Mutex::~Mutex() {
	pthread_mutex_destroy(&_mutex);
}

void Mutex::lock() {
	pthread_mutex_lock(&_mutex);
}

void Mutex::unlock() {
	pthread_mutex_unlock(&_mutex);
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
