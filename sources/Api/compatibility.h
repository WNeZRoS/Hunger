#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#ifdef UNICODE
	#define XCHAR wchar_t
	#define XSTRING wstring
	#define XSTRINGSTREAM wstringstream
	#define xstrlen wcslen
	#define _T(x) L##x
#else
	#define XCHAR char
	#define XSTRING string
	#define XSTRINGSTREAM stringstream
	#define xstrlen strlen
	#define _T(x) x
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef _MSC_VER
#define INFINITY (DBL_MAX+DBL_MAX)
#define NAN (INFINITY-INFINITY)
#endif

short mabs(short x);
int mabs(int x);
long mabs(long x);
float mabs(float x);
double mabs(double x);
bool isNaN(float x);

typedef unsigned long Timestamp;

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
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

Timestamp getCurrentTime();
void getTime(Time& time);
void msSleep(unsigned int millis);

#endif // COMPATIBILITY_H
