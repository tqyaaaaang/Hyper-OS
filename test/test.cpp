/**
 * test/test.h
 * Unittest entry
 */

#include "test.h"
#include "test_interrupt.h"
#include "test_pmm.h"
#include "test_pm.h"
#include "test_ffma.h"
#include "../src/mm/pmm.h"
#include "../src/logging/logging.h"
#include "../src/utils/allocator/ffma.h"

void test()
{
	logging::info << "Testing the system" << logging::log_endl;

	test_interrupt ();
	test_pm();
	test_pmm();
    test_ffma();
}