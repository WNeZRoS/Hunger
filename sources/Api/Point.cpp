#include "Point.h"
#include "Logger.h"

Point::Point() { this->x = this->y = 0; }
Point::Point(int x, int y) { this->x = x; this->y = y; }
bool Point::operator == (const Point& p) const { return x == p.x && y == p.y; }
bool Point::operator != (const Point& p) const { return !(*this == p); }

Point& Point::operator = (const Point& p) { x = p.x; y = p.y; return *this; }
Point& Point::operator = (const int& v) { x = v; y = v; return *this; }

Point& Point::operator += (const Point& p) { x += p.x; y += p.y; return *this; }
Point& Point::operator -= (const Point& p) { x -= p.x; y -= p.y; return *this; }
Point& Point::operator *= (const Point& p) { x *= p.x; y *= p.y; return *this; }
Point& Point::operator /= (const Point& p) { x /= p.x; y /= p.y; return *this; }

Point& Point::operator += (const int& v) { x += v; y += v; return *this; }
Point& Point::operator -= (const int& v) { x -= v; y -= v; return *this; }
Point& Point::operator *= (const int& v) { x *= v; y *= v; return *this; }
Point& Point::operator /= (const int& v) { x /= v; y /= v; return *this; }

const Point Point::operator + (const Point& p) const { return Point(*this) += p; }
const Point Point::operator - (const Point& p) const { return Point(*this) -= p; }
const Point Point::operator * (const Point& p) const { return Point(*this) *= p; }
const Point Point::operator / (const Point& p) const { return Point(*this) /= p; }

const Point Point::operator + (const int& v) const { return Point(*this) += v; }
const Point Point::operator - (const int& v) const { return Point(*this) -= v; }
const Point Point::operator * (const int& v) const { return Point(*this) *= v; }
const Point Point::operator / (const int& v) const { return Point(*this) /= v; }

std::istream& operator >> (std::istream& in, Point& p) {
	in >> p.x >> p.y;
	return in;
}

std::ostream& operator << (std::ostream& out, const Point& p) {
	out << "(" << p.x << ", " <<  p.y << ")";
	return out;
}

Log::BasicLogger& operator << (Log::BasicLogger& out, const Point& p) {
	out << "(" << p.x << ", " <<  p.y << ")";
	return out;
}
