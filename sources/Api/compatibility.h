#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#ifdef UNICODE
	#define CHAR wchar_t
	#define _T(x) L##x
#else
	#define CHAR char
	#define _T(x) x
#endif

#define ABS(a) (a < 0 ? -(a) : a)

unsigned long long getCurrentTime();

#endif // COMPATIBILITY_H
