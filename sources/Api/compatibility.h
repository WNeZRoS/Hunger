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
private:
	unsigned int _size;
	T* _items;

public:
	Array() {
		_size = 0;
		_items = NULL;
	}

	Array(unsigned int size) {
		if(size > 0) {
			_items = new T[size];
			this->_size = size;
		} else {
			this->_size = 0;
			this->_items = NULL;
		}
	}

	~Array() {
		if(_items) delete [] _items;
	}

	void rebuild(unsigned int size) {
		if(_items) delete [] _items;
		if(size > 0) {
			_items = new T[size];
			this->_size = size;
		} else {
			this->_size = 0;
			this->_items = NULL;
		}
	}

	void copy(const T* array, unsigned int size) {
		if(this->_items) delete [] _items;

		this->_size = size;
		this->_items = new T[this->_size];
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = array[i];
	}

	bool contains(const T& p) const {
		for(unsigned int i = 0; i < _size; i++)
			if(_items[i] == p) return true;
		return false;
	}

	Array<T>& operator = (const Array<T>& array) {
		if(this == &array) return *this;

		if(this->_items) delete [] _items;

		this->_size = array._size;
		this->_items = new T[this->_size];
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = array._items[i];
		return *this;
	}

	const T& operator [] (unsigned int index) const {
		return _items[index];
	}

	T& operator [] (unsigned int index) {
		return _items[index];
	}

	unsigned int size() const { return _size; }
};

#endif // COMPATIBILITY_H
