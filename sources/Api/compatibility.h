#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#ifdef UNICODE
	#define XCHAR wchar_t
	#define XSTRING wstring
	#define _T(x) L##x
#else
	#define XCHAR char
	#define XSTRING string
	#define _T(x) x
#endif

#define ABS(a) (a < 0 ? -(a) : a)

#ifndef NULL
#define NULL 0
#endif

unsigned long long getCurrentTime();

#endif // COMPATIBILITY_H
