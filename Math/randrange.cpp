#include "randrange.h"
#include <cmath>
float randrange(float a, float b)
{
return ((float)rand()) / ((float)RAND_MAX) * (b - a) + a;
}