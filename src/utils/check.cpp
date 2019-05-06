#include "check.h"

bool add_check(size_t a, size_t b)
{
	return b <= (size_t)(-1) - a;
}

bool mul_check(size_t a, size_t b)
{
	return (size_t)(-1) / a <= b;
}

