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

typedef unsigned long long Timestamp;

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
bool equal(float a, float b);
void msSleep(unsigned int millis);

template <typename T>
struct Array
{
	unsigned int size;
	T* items;

	Array() {
		size = 0;
		items = NULL;
	}

	Array(unsigned int size) {
		if(size > 0) {
			items = new T[size];
			this->size = size;
		} else {
			this->size = 0;
			this->items = NULL;
		}
	}

	~Array() {
		if(items) delete [] items;
	}

	void rebuild(unsigned int size) {
		if(items) delete [] items;
		if(size > 0) {
			items = new T[size];
			this->size = size;
		} else {
			this->size = 0;
			this->items = NULL;
		}
	}

	void copy(const T* array, unsigned int size) {
		if(this->items) delete [] items;

		this->size = size;
		this->items = new T[this->size];
		for(unsigned int i = 0; i < this->size; i++)
			this->items[i] = array[i];
	}

	bool contains(const T& p) const {
		for(unsigned int i = 0; i < size; i++)
			if(items[i] == p) return true;
		return false;
	}

	Array<T>& operator = (const Array<T>& array) {
		if(this == &array) return *this;

		if(this->items) delete [] items;

		this->size = array.size;
		this->items = new T[this->size];
		for(unsigned int i = 0; i < this->size; i++)
			this->items[i] = array.items[i];
		return *this;
	}

	const T& operator [] (unsigned int index) const {
		return items[index];
	}

	T& operator [] (unsigned int index) {
		return items[index];
	}
};

#endif // COMPATIBILITY_H
