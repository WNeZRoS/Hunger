#ifndef POINT_H
#define POINT_H

#include <fstream>

namespace Log
{
	class BasicLogger;
};

struct Point_i;

struct Point
{
	float x, y;

	Point();
	Point(float x, float y);
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
};

struct Point_i
{
	int x, y;

	Point_i();
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
	Point toPoint() const;

	friend std::istream& operator >> (std::istream& in, Point_i& p);
	friend std::ostream& operator << (std::ostream& out, const Point_i& p);
	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Point_i& p);
};

#endif // Point_i_H
