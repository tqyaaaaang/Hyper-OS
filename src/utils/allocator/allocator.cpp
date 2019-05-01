#include "allocator.h"

allocator::allocator()
{}

allocator::~allocator()
{}

allocator::allocator(size_t n)
{
	set_size(n);
}

void allocator::set_size(size_t n)
{
	this->n = n;
}

int allocator::get_size()
{
	return n;
}

allocator::segment::segment()
{
	start = 0, length = 0;
}

void allocator::free(const allocator::segment &seg)
{
	this->free(seg.start, seg.length);
}

allocator::segment::segment(size_t start, size_t length)
{
	this->start = start;
	this->length = length;
}
