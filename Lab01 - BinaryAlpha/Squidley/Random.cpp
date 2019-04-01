#include "Random.h"
#include <stdlib.h>

float Random::randomFloat()
{
	return (float)rand() / RAND_MAX;
}

float Random::randomInRange(float min, float max)
{
	return randomFloat() * (max - min + 1) + min;
}