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

struct Time {
	unsigned short year;
	unsigned short month;
	unsigned short dayOfWeek;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	unsigned short milliseconds;
};

unsigned long long getCurrentTime();
void getTime(Time& time);

#endif // COMPATIBILITY_H
