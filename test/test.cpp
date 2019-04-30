/**
 * test/test.h
 * Unittest entry
 */

#include "test.h"
#include "test_interrupt.h"
#include "../src/mm/pmm.h"
#include "../src/logging/logging.h"

void test()
{
	logging::info << "Testing the system" << logging::log_endl;

	test_interrupt ();
	debug_pmm();
}
