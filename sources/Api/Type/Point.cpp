#include "Point.h"
#include "../Core/Logger.h"
#include "../compatibility.h"
#include <iomanip>
#include <cmath>

const Point Point::ONE(1.0f, 0.0f);
const Point Point::iONE(0.0f, 1.0f);
const Point Point::dONE(1.0f, 1.0f);
const Point Point::ZERO(0.0f, 0.0f);
const Point Point::NaN(NAN, NAN);

Point::Point() { this->x = this->y = 0; }
Point::Point(const Point_i &point) { this->x = point.x; this->y = point.y; }
Point::Point(float x, float y) { this->x = x; this->y = y; }
Point::Point(Float x, Float y) { this->x = x; this->y = y; }
bool Point::operator == (const Point& p) const { return x == p.x && y == p.y; }
bool Point::operator != (const Point& p) const { return !(*this == p); }
bool Point::operator == (const float& v) const { return x == v && y == v; }
bool Point::operator != (const float& v) const { return !(*this == v); }

bool Point::operator > (const Point& p) const { return x > p.x && y > p.y; }
bool Point::operator >= (const Point& p) const { return x >= p.x && y >= p.y; }

bool Point::operator < (const Point& p) const { return x < p.x && y < p.y; }
bool Point::operator <= (const Point& p) const { return x <= p.x && y <= p.y; }

Point& Point::operator = (const Point& p) { x = p.x; y = p.y; return *this; }
Point& Point::operator = (const float& v) { x = v; y = v; return *this; }

Point& Point::operator += (const Point& p) { x += p.x; y += p.y; return *this; }
Point& Point::operator -= (const Point& p) { x -= p.x; y -= p.y; return *this; }
Point& Point::operator *= (const Point& p) { x *= p.x; y *= p.y; return *this; }
Point& Point::operator /= (const Point& p) { x /= p.x; y /= p.y; return *this; }

Point& Point::operator += (const float& v) { x += v; y += v; return *this; }
Point& Point::operator -= (const float& v) { x -= v; y -= v; return *this; }
Point& Point::operator *= (const float& v) { x *= v; y *= v; return *this; }
Point& Point::operator /= (const float& v) { x /= v; y /= v; return *this; }

const Point Point::operator + (const Point& p) const { return Point(*this) += p; }
const Point Point::operator - (const Point& p) const { return Point(*this) -= p; }
const Point Point::operator * (const Point& p) const { return Point(*this) *= p; }
const Point Point::operator / (const Point& p) const { return Point(*this) /= p; }

const Point Point::operator + (const float& v) const { return Point(*this) += v; }
const Point Point::operator - (const float& v) const { return Point(*this) -= v; }
const Point Point::operator * (const float& v) const { return Point(*this) *= v; }
const Point Point::operator / (const float& v) const { return Point(*this) /= v; }

Point Point::abs() const { return Point(x.abs(), y.abs()); }
float Point::length() const { return (x.sq() + y.sq()).sqrt().f(); }
bool Point::nan() const { return x.isNaN() || y.isNaN(); }
Point_i Point::toPoint_i() const { return Point_i(x.f(), y.f()); }

std::istream& operator >> (std::istream& in, Point& p) {
	in >> p.x >> p.y;
	return in;
}

std::ostream& operator << (std::ostream& out, const Point& p) {
	out.setf(std::ios::fixed, std::ios::floatfield);
	out.setf(std::ios::showpoint);
	out << "(" << std::setprecision(2) << p.x << ", " << std::setprecision(2) << p.y << ")";
	return out;
}

Log::BasicLogger& operator << (Log::BasicLogger& out, const Point& p) {
	out << "(" << std::setprecision(2) << p.x << ", " << std::setprecision(2) << p.y << ")";
	return out;
}

// Point_i

const Point_i Point_i::ONE(1, 0);
const Point_i Point_i::iONE(0, 1);
const Point_i Point_i::dONE(1, 1);
const Point_i Point_i::ZERO(0, 0);

Point_i::Point_i() { this->x = this->y = 0; }
Point_i::Point_i(const Point &point) { this->x = point.x.f(); this->y = point.y.f(); }
Point_i::Point_i(int x, int y) { this->x = x; this->y = y; }
bool Point_i::operator == (const Point_i& p) const { return x == p.x && y == p.y; }
bool Point_i::operator != (const Point_i& p) const { return !(*this == p); }
bool Point_i::operator == (const int& v) const { return x == v && y == v; }
bool Point_i::operator != (const int& v) const { return !(*this == v); }

bool Point_i::operator > (const Point_i& p) const { return x > p.x && y > p.y; }
bool Point_i::operator >= (const Point_i& p) const { return x >= p.x && y >= p.y; }

bool Point_i::operator < (const Point_i& p) const { return x < p.x && y < p.y; }
bool Point_i::operator <= (const Point_i& p) const { return x <= p.x && y <= p.y; }

Point_i& Point_i::operator = (const Point_i& p) { x = p.x; y = p.y; return *this; }
Point_i& Point_i::operator = (const int& v) { x = v; y = v; return *this; }

Point_i& Point_i::operator += (const Point_i& p) { x += p.x; y += p.y; return *this; }
Point_i& Point_i::operator -= (const Point_i& p) { x -= p.x; y -= p.y; return *this; }
Point_i& Point_i::operator *= (const Point_i& p) { x *= p.x; y *= p.y; return *this; }
Point_i& Point_i::operator /= (const Point_i& p) { x /= p.x; y /= p.y; return *this; }

Point_i& Point_i::operator += (const int& v) { x += v; y += v; return *this; }
Point_i& Point_i::operator -= (const int& v) { x -= v; y -= v; return *this; }
Point_i& Point_i::operator *= (const int& v) { x *= v; y *= v; return *this; }
Point_i& Point_i::operator /= (const int& v) { x /= v; y /= v; return *this; }

const Point_i Point_i::operator + (const Point_i& p) const { return Point_i(*this) += p; }
const Point_i Point_i::operator - (const Point_i& p) const { return Point_i(*this) -= p; }
const Point_i Point_i::operator * (const Point_i& p) const { return Point_i(*this) *= p; }
const Point_i Point_i::operator / (const Point_i& p) const { return Point_i(*this) /= p; }

const Point_i Point_i::operator + (const int& v) const { return Point_i(*this) += v; }
const Point_i Point_i::operator - (const int& v) const { return Point_i(*this) -= v; }
const Point_i Point_i::operator * (const int& v) const { return Point_i(*this) *= v; }
const Point_i Point_i::operator / (const int& v) const { return Point_i(*this) /= v; }

Point_i Point_i::abs() const { return Point_i(mabs(x), mabs(y)); }
float Point_i::length() const { return std::sqrt(static_cast<float>(x * x + y * y)); }
unsigned int Point_i::qlength() const { return mabs(x) + mabs(y); }
Point Point_i::toPoint() const { return Point(x, y); }

std::istream& operator >> (std::istream& in, Point_i& p) {
	in >> p.x >> p.y;
	return in;
}

std::ostream& operator << (std::ostream& out, const Point_i& p) {
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

Log::BasicLogger& operator << (Log::BasicLogger& out, const Point_i& p) {
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}
