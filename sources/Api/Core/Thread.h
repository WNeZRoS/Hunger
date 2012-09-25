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

#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
	void _lock(const char *file, int line);
	bool _onelock(const char *file, int line);
	bool _trylock(const char *file, int line);
#else
	void lock();
	bool onelock();
	bool trylock();
#endif
	void unlock();

	friend class Cond;

private:
	pthread_mutex_t _mutex;
	pthread_t _thread;
	bool _locked;
#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
	const char *_file;
	int _line;
#endif
};

class FuncMutex {
public:
#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
	FuncMutex(Mutex& mutex, const char *file, int line);
#else
	FuncMutex(Mutex& mutex);
#endif
	~FuncMutex();
private:
	Mutex *_mutex;
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

#if !defined(NO_MACRO_LOCK) && (defined(DEBUG) || defined(_DEBUG))
#define lock() _lock(__FILE__, __LINE__)
#define onelock() _onelock(__FILE__, __LINE__)
#define trylock() _trylock(__FILE__, __LINE__)
#define LocalMutex(var, mutex) FuncMutex var(mutex, __FILE__, __LINE__)
#else
#define LocalMutex(var, mutex) FuncMutex var(mutex)
#endif

#endif // THREAD_H
