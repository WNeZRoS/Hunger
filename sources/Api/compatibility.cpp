#include "compatibility.h"

#ifdef WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <iostream>
#include <cmath>

#define ABS(x) mabs(x)

short mabs(short x) { if(x < 0) return -x; return x; }
int mabs(int x) { if(x < 0) return -x; return x; }
long mabs(long x) { if(x < 0) return -x; return x; }
float mabs(float x) { if(x < 0) return -x; return x; }
double mabs(double x) { if(x < 0) return -x; return x; }

bool isNaN(float x) {
#ifdef _MSC_VER
	return _isnan(x);
#else
#if ANDROID_NDK
	return isnan(x);
#else
	return std::isnan(x);
#endif
#endif
}

Timestamp getCurrentTime() {
#ifdef WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	Timestamp t = (time(NULL) * 1000 + st.wMilliseconds);
	return t;
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000);
#endif
}

void getTime(Time& currentTime) {
#ifdef WIN32
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	currentTime.year = sm.wYear;
	currentTime.month = sm.wMonth;
	currentTime.dayOfWeek = sm.wDayOfWeek;
	currentTime.day = sm.wDay;
	currentTime.hour = sm.wHour;
	currentTime.minute = sm.wMinute;
	currentTime.second = sm.wSecond;
	currentTime.milliseconds = sm.wMilliseconds;
#else
	time_t timet = time(NULL);
	tm *local = localtime(&timet);
	currentTime.year = local->tm_year;
	currentTime.month = local->tm_mon;
	currentTime.dayOfWeek = local->tm_wday;
	currentTime.day = local->tm_mday;
	currentTime.hour = local->tm_hour;
	currentTime.minute = local->tm_min;
	currentTime.second = local->tm_sec;
	currentTime.milliseconds = getCurrentTime() % 1000;
#endif
}

void msSleep(unsigned int millis) {
#ifdef WIN32
	Sleep(millis);
#else
	timespec tm;
	tm.tv_sec = 0;
	tm.tv_nsec = millis * 1000000 + 1;
	nanosleep(&tm, NULL);
#endif
}
