#include "Random.h"
#include <cstdlib>

void Random::start(int x) {
	::srand(x);
}

int Random::rand(int max) {
	return ::rand() % max;
}

int Random::rand(int min, int max) {
	return Random::rand(max - min) + min;
}

int Random::rand(const int* values, const int* chance, int size) {
	if(size <= 0 || !values || !chance) return 0;

	int total = 0;
	for(int i = 0; i < size; i++) total += chance[i];

	int r = Random::rand(total);
	total = 0;
	for(int i = 0; i < size; i++) {
		if(r >= total && r < total + chance[i]) return values[i];
		total += chance[i];
	}

	return values[0];
}
