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
#include "../src/interrupt/interrupts/double_internal_test.h"

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
	int return_value;

	{
		internal_test_interrupt current_interrupt;
		return_value = interrupt ( &current_interrupt );
		logging::info << "Test on INTERNAL TEST interrupt succeeded, return value : " << return_value << logging::log_endl;
	}

	{
		double_internal_test_interrupt current_interrupt;
		return_value = interrupt ( &current_interrupt );
		logging::info << "Test on DOUBLE INTERNAL TEST interrupt succeeded, return value : " << return_value << logging::log_endl;
	}
}
