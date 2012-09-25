#ifndef ARRAY_H
#define ARRAY_H

#include <vector>
#include <list>
#include <fstream>
#include "../Core/Logger.h"

template <typename T>
class Array
{
public:
	Array() {
		_size = 0;
		_items = NULL;
	}

	Array(const Array<T> &array) {
		_size = 0;
		_items = NULL;
		this->copy(array._items, array._size);
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

	bool contains(const T& p) const {
		for(unsigned int i = 0; i < _size; i++)
			if(_items[i] == p) return true;
		return false;
	}

	void copy(const T* array, unsigned int size) {
		if(this->_items == array) return;

		if(this->_items) delete [] _items;

		this->_size = size;
		this->_items = new T[this->_size];
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = array[i];
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

	Array<T>& operator = (const Array<T>& array) {
		if(this == &array) return *this;

		if(this->_items) delete [] _items;

		this->_size = array._size;
		this->_items = new T[this->_size];
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = array._items[i];
		return *this;
	}

	Array<T>& operator = (const std::vector<T>& vector) {
		if(this->_items) delete [] _items;

		this->_size = vector.size();
		this->_items = new T[this->_size];
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = vector.at(i);
		return *this;
	}

	Array<T>& operator = (const std::list<T>& l) {
		if(this->_items) delete [] _items;

		this->_size = l.size();
		this->_items = new T[this->_size];
		unsigned int i = 0;

		typedef typename std::list<T>::iterator iterator_type;

		for(iterator_type it = l.begin(); it != l.end(); it++)
			this->_items[i++] = *it;
		return *this;
	}

	template <typename AnyT>
	Array<T>& operator = (const AnyT& value) {
		for(unsigned int i = 0; i < this->_size; i++)
			this->_items[i] = value;
		return *this;
	}

	const T& operator [] (unsigned int index) const {
		return _items[index];
	}

	T& operator [] (unsigned int index) {
		return _items[index];
	}

	T& last() {
		if(_size == 0) return NULL;
		return _items[_size - 1];
	}

	const T& last() const {
		if(_size == 0) return NULL;
		return _items[_size - 1];
	}

	unsigned int size() const {
		return _size;
	}

	friend std::ostream& operator << (std::ostream& out,  const Array<T>& array) {
		out << "Array(" << array._size << ") [ ";
		for(unsigned int i = 0; i < array._size; i++) {
			if(i > 0) out << ", ";
			out << "{ " << array._items[i] << " }";
		}
		out << " ]";

		return out;
	}

	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Array<T>& array) {
		out << "Array(" << array._size << ") [ ";
		for(unsigned int i = 0; i < array._size; i++) {
			if(i > 0) out << ", ";
			out << "{ " << array._items[i] << " }";
		}
		out << " ]";

		return out;
	}

private:
	unsigned int _size;
	T* _items;

};

template <typename TT>
struct TwoDimArray {

	typedef Array< Array<TT> > T;

	static T array(unsigned int size) {
		return array(size, 0);
	}

	static T array(unsigned int height, unsigned int width) {
		T result(height);

		for(unsigned int i = 0; i < result.size(); i++)
			result[i].rebuild(width);

		return result;
	}
};

#endif // ARRAY_H
