#include "compatibility.h"

#ifdef WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

unsigned long long getCurrentTime() {
#ifdef WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	return (time(NULL) * 1000 + st.wMilliseconds);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec/1000);
#endif
}

