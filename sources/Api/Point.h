#ifndef POINT_H
#define POINT_H

#include <fstream>

namespace Log
{
	class BasicLogger;
};

struct Point
{
	int x, y;

	Point();
	Point(int x, int y);
	bool operator == (const Point& p) const;
	bool operator != (const Point& p) const;

	bool operator > (const Point& p) const;
	bool operator >= (const Point& p) const;
	bool operator < (const Point& p) const;
	bool operator <= (const Point& p) const;

	Point& operator = (const Point& p);
	Point& operator = (const int& v);

	Point& operator += (const Point& p);
	Point& operator -= (const Point& p);
	Point& operator *= (const Point& p);
	Point& operator /= (const Point& p);

	Point& operator += (const int& v);
	Point& operator -= (const int& v);
	Point& operator *= (const int& v);
	Point& operator /= (const int& v);

	const Point operator + (const Point& p) const;
	const Point operator - (const Point& p) const;
	const Point operator * (const Point& p) const;
	const Point operator / (const Point& p) const;

	const Point operator + (const int& v) const;
	const Point operator - (const int& v) const;
	const Point operator * (const int& v) const;
	const Point operator / (const int& v) const;

	friend std::istream& operator >> (std::istream& in, Point& p);
	friend std::ostream& operator << (std::ostream& out, const Point& p);
	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Point& p);
};

#endif // POINT_H
