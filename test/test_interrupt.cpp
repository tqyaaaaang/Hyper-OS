/**
 * test/test_interrupt.h
 * Test interrupt
 */

#include "test_interrupt.h"

#include <cassert>
#include "../src/logging/logging.h"
#include "../src/core/core.h"
#include "../src/status/status.h"
#include "../src/interrupt/interrupt.h"
#include "../src/interrupt/interrupts/internal_test.h"

void test_interrupt ()
{
	logging::debug << "Testing interrupts" << logging::log_endl;

	status.get_core ()->acquire ();
	test_internal_interrupt ();
	status.get_core ()->release ();
}

void test_internal_interrupt ()
{
	logging::debug << "Testing internal interrupts" << logging::log_endl;

	int return_value = interrupt ( new internal_test_interrupt () );
	logging::debug << "Testing INTERNAL TEST interrupt, return value : " << return_value << logging::log_endl;
}
