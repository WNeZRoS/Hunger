#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	static void start(int x);

	static int rand(int max);
	static int rand(int min, int max);
	static int rand(const int* values, const int* chance, int size);
};

#endif // RANDOM_H
