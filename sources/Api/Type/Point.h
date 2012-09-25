#ifndef POINT_H
#define POINT_H

#include "Float.h"
#include <fstream>

namespace Log
{
	class BasicLogger;
}

struct Point_i;

struct Point
{
	Float x, y;

	Point();
	Point(const Point_i& point);
	Point(float x, float y);
	Point(Float x, Float y);
	bool operator == (const Point& p) const;
	bool operator != (const Point& p) const;
	bool operator == (const float& v) const;
	bool operator != (const float& v) const;

	bool operator > (const Point& p) const;
	bool operator >= (const Point& p) const;
	bool operator < (const Point& p) const;
	bool operator <= (const Point& p) const;

	Point& operator = (const Point& p);
	Point& operator = (const float& v);

	Point& operator += (const Point& p);
	Point& operator -= (const Point& p);
	Point& operator *= (const Point& p);
	Point& operator /= (const Point& p);

	Point& operator += (const float& v);
	Point& operator -= (const float& v);
	Point& operator *= (const float& v);
	Point& operator /= (const float& v);

	const Point operator + (const Point& p) const;
	const Point operator - (const Point& p) const;
	const Point operator * (const Point& p) const;
	const Point operator / (const Point& p) const;

	const Point operator + (const float& v) const;
	const Point operator - (const float& v) const;
	const Point operator * (const float& v) const;
	const Point operator / (const float& v) const;

	Point abs() const;
	float length() const;
	bool nan() const;
	Point_i toPoint_i() const;

	friend std::istream& operator >> (std::istream& in, Point& p);
	friend std::ostream& operator << (std::ostream& out, const Point& p);
	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Point& p);

	static const Point ONE;
	static const Point iONE;
	static const Point dONE;
	static const Point ZERO;
	static const Point NaN;
};

struct Point_i
{
	int x, y;

	Point_i();
	Point_i(const Point& point);
	Point_i(int x, int y);
	bool operator == (const Point_i& p) const;
	bool operator != (const Point_i& p) const;
	bool operator == (const int& v) const;
	bool operator != (const int& v) const;

	bool operator > (const Point_i& p) const;
	bool operator >= (const Point_i& p) const;
	bool operator < (const Point_i& p) const;
	bool operator <= (const Point_i& p) const;

	Point_i& operator = (const Point_i& p);
	Point_i& operator = (const int& v);

	Point_i& operator += (const Point_i& p);
	Point_i& operator -= (const Point_i& p);
	Point_i& operator *= (const Point_i& p);
	Point_i& operator /= (const Point_i& p);

	Point_i& operator += (const int& v);
	Point_i& operator -= (const int& v);
	Point_i& operator *= (const int& v);
	Point_i& operator /= (const int& v);

	const Point_i operator + (const Point_i& p) const;
	const Point_i operator - (const Point_i& p) const;
	const Point_i operator * (const Point_i& p) const;
	const Point_i operator / (const Point_i& p) const;

	const Point_i operator + (const int& v) const;
	const Point_i operator - (const int& v) const;
	const Point_i operator * (const int& v) const;
	const Point_i operator / (const int& v) const;

	Point_i abs() const;
	float length() const;
	unsigned int qlength() const;
	int sign() const;
	Point toPoint() const;

	friend std::istream& operator >> (std::istream& in, Point_i& p);
	friend std::ostream& operator << (std::ostream& out, const Point_i& p);
	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Point_i& p);

	static const Point_i ONE;
	static const Point_i iONE;
	static const Point_i dONE;
	static const Point_i ZERO;
};

#endif // Point_i_H
