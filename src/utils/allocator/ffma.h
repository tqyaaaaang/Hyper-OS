/*
 * First-Fit memory allocation
 */
#pragma once
#include "allocator.h"
#include <list>
#include <cstddef>
#include <cstdint>

class ffma : public allocator {

	std::list<allocator::segment> free_list;

	void merge(std::list<allocator::segment>::iterator seg);
	
public:

	ffma(size_t n);
	virtual size_t malloc(size_t len);
	virtual void free(size_t addr, size_t len);
};

