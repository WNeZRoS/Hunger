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
	void unlock();

	friend class Cond;

private:
	pthread_mutex_t _mutex;
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
