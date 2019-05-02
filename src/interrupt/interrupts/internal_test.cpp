/**
 * interrupt/interrupts/internal_test.cpp
 * Interrupt classes
 * Internal test interrupt
 */

#include "internal_test.h"

#include "../../logging/logging.h"

internal_test_interrupt::internal_test_interrupt ()
	: internal_interrupt_t ( interrupt_id_t::INTERNAL_TEST )
{
}

void internal_test_interrupt::process ()
{
	logging::info << "Running ISR of INTERNAL TEST interrupt" << logging::log_endl;
}
