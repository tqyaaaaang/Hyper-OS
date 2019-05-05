#include "test_pm.h"
#include "../src/mm/pmm.h"
#include "../src/mm/pmem.h"
#include "../src/logging/logging.h"
#include <cassert>

using logging::debug;
using logging::log_endl;
using pm::read;
using pm::write;

void test_pm()
{
	write(10, 1);
	assert(read(10) == 1);
	write(1 << 20, 127);
	write(1 << 29 | 1, 2);
	assert(read(1 << 20) == 127);
	assert(read(1 << 29 | 1) == 2);
	write(10, 0);
	write(1 << 20, 0);
	write(1 << 29 | 1, 0);
	debug << "pm check ok." << log_endl;
}
