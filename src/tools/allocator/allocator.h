/*
 * Interface for General Memory Allocator
 */

class Allocator {

    size_t n;

public:

    Allocator(size_t);
	virtual ~Allocator();

	// alloc continous memory with length @len
	// return start position
	virtual size_t alloc(size_t len) = 0;

	// free continous memory start frome @addr
	virtual void free(size_t addr) = 0;
	
};
