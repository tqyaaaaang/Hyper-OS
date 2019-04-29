/*
 * Interface for General Memory Allocator
 */

#pragma once
#include <cstddef>
#include <cstdint>

class allocator {

    size_t n;

public:

	allocator();
    allocator(size_t);
	virtual ~allocator();

	// set memory size
	void set_size(size_t n);
	int get_size();
	
	// alloc continous memory with length @len
	// return start position
	virtual size_t malloc(size_t len) = 0;

	// free continous memory start frome @addr
	// with length @len
	// limited correction check for @len 
	virtual void free(size_t addr, size_t len) = 0;

	struct segment {
		size_t start;
		size_t length;
		segment();
		segment(size_t, size_t);
	};
	
	void free(const segment &seg);
};

