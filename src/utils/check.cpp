#include "check.h"
#include "../mm/pmem_info.h"
#include "../env/env.h"

bool add_check(size_t a, size_t b)
{
	return b <= (size_t)(-1) - a;
}

bool mul_check(size_t a, size_t b)
{
	return (size_t)(-1) / a >= b;
}

void round2page(size_t &size)
{
	size = (size / PAGE_SIZE
			+ (size % PAGE_SIZE != 0)) * PAGE_SIZE;
}
