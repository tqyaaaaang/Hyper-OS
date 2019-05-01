#include "test_ffma.h"
#include "../src/utils/allocator/ffma.h"
#include "../src/logging/logging.h"
#include <cassert>

using logging::debug;
using logging::log_endl;
typedef allocator::segment segment;

void test_ffma()
{
	allocator *ffma_allocator = new ffma(100);
	segment segs[10];
	for (int i = 0; i < 10; i++) {
		segs[i] = segment(ffma_allocator->malloc(i + 1), i+1);
		assert(segs[i].start == i*(i+1)/2);
	}
	int ans[] = {63, 54, 46, 6, 38, 15, 21, 28, 36, 45};
	for (int i = 9; i >= 0; i--) {
		ffma_allocator->free(segs[i]);
		segs[i] = segment(ffma_allocator->malloc(10 - i), 10 - i);
		assert(segs[i].start == ans[i]);
	}
	delete ffma_allocator;
	debug << "ffma check ok." << log_endl;
}
