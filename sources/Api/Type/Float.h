#ifndef FLOAT_H
#define FLOAT_H

#include <fstream>

namespace Log
{
	class BasicLogger;
}

struct Float
{
public:
	float value;

	Float();
	Float(const Float& x);
	Float(float x);
	~Float();

	bool operator == (const Float& p) const;
	bool operator != (const Float& p) const;
	bool operator == (const float& x) const;
	bool operator != (const float& x) const;

	bool operator > (const Float& p) const;
	bool operator >= (const Float& p) const;
	bool operator < (const Float& p) const;
	bool operator <= (const Float& p) const;

	Float& operator = (const Float& p);
	Float& operator = (const float& v);

	Float& operator += (const Float& p);
	Float& operator -= (const Float& p);
	Float& operator *= (const Float& p);
	Float& operator /= (const Float& p);

	const Float operator + (const Float& p) const;
	const Float operator - (const Float& p) const;
	const Float operator * (const Float& p) const;
	const Float operator / (const Float& p) const;

	Float abs() const;
	Float sq() const;
	Float pow(float p) const;
	Float sqrt() const;
	Float root(float p) const;
	bool isNaN() const;
	int sign() const;

	float& f();
	const float& f() const;

	static bool equal(float a, float b);
	static bool greater(float a, float b);
	static bool greaterOrEqual(float a, float b);

	friend std::istream& operator >> (std::istream& in, Float& f);
	friend std::ostream& operator << (std::ostream& out, const Float& f);
	friend Log::BasicLogger& operator << (Log::BasicLogger& out, const Float& f);
};

#endif // FLOAT_H
