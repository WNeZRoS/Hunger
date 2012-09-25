#include "Float.h"
#include "../compatibility.h"
#include "../Core/Logger.h"
#include <limits>
#include <cmath>

Float::Float() { value = 0.0f; }
Float::Float(const Float& x) { value = x.value; }
Float::Float(float x) { value = x; }
Float::~Float() { }

bool Float::operator == (const Float& x) const {
	return Float::equal(value, x.value);
}
bool Float::operator != (const Float& x) const {
	return !(*this == x);
}

bool Float::operator == (const float& x) const {
	return Float::equal(value, x);
}
bool Float::operator != (const float& x) const {
	return !(*this == x);
}

bool Float::operator > (const Float& x) const {
	return Float::greater(value, x.value);
}

bool Float::operator >= (const Float& x) const {
	return Float::greaterOrEqual(value, x.value);
}

bool Float::operator < (const Float& x) const {
	return Float::greater(x.value, value);
}

bool Float::operator <= (const Float& x) const {
	return Float::greaterOrEqual(x.value, value);
}

Float& Float::operator = (const Float& x) {
	value = x.value;
	return *this;
}

Float& Float::operator = (const float& v) {
	value = v;
	return *this;
}

Float& Float::operator += (const Float& x) {
	value += x.value;
	return *this;
}

Float& Float::operator -= (const Float& x) {
	value -= x.value;
	return *this;
}

Float& Float::operator *= (const Float& x) {
	value *= x.value;
	return *this;
}

Float& Float::operator /= (const Float& x) {
	value /= x.value;
	return *this;
}

const Float Float::operator + (const Float& x) const {
	return Float(value + x.value);
}

const Float Float::operator - (const Float& x) const {
	return Float(value - x.value);
}

const Float Float::operator * (const Float& x) const {
	return Float(value * x.value);
}

const Float Float::operator / (const Float& x) const {
	return Float(value / x.value);
}

Float Float::abs() const {
	return Float(std::fabs(value));
}

Float Float::sq() const {
	return Float(value * value);
}

Float Float::pow(float p) const {
	return Float(std::pow(value, p));
}

Float Float::sqrt() const {
	return Float(std::sqrt(value));
}

Float Float::root(float p) const {
	return Float(std::pow(value, 1.0f / p));
}

bool Float::isNaN() const {
	return ::isNaN(value);
}

int Float::sign() const {
	if(value < 0) return -1;
	if(value > 0) return 1;
	return 0;
}

bool Float::equal(float a, float b) {
	return std::fabs(a - b) < 2 * std::numeric_limits<float>::epsilon();
}

bool Float::greater(float a, float b) {
	return a - b > std::numeric_limits<float>::epsilon();
}

bool Float::greaterOrEqual(float a, float b) {
	return a - b > -std::numeric_limits<float>::epsilon();
}

float& Float::f() {
	return value;
}

const float& Float::f() const {
	return value;
}

std::istream& operator >> (std::istream& in, Float& f) {
	in >> f.value;
	return in;
}

std::ostream& operator << (std::ostream& out, const Float& f) {
	out << f.value;
	return out;
}

Log::BasicLogger& operator << (Log::BasicLogger& out, const Float& f) {
	out << f.value;
	return out;
}
