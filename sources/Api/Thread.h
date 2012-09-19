#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread
{
public:
	Thread();
	virtual ~Thread() = 0;

	bool isRunning() const;
	virtual void run();
	void stop();
	void wait();

private:
	pthread_t _thread;
	bool _running;

	static void * ThreadFunc(void *arg);
};

class Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	bool lock(void * locker);
	void unlock();

	friend class Cond;

private:
	pthread_mutex_t _mutex;
	void *_locker;
#ifdef DEBUG
	pthread_t _thread;
	bool _locked;
#endif
};

class FakeMutex
{
public:
	FakeMutex() {}
	~FakeMutex() {}

	void lock() {}
	void unlock() {}
};

class Cond
{
public:
	Cond();
	~Cond();

	void wait(Mutex& mutex);
	void signal();

private:
	pthread_cond_t _cond;
};

#endif // THREAD_H
