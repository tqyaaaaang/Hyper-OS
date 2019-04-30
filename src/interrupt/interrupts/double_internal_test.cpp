/**
 * interrupt/interrupts/double_internal_test.cpp
 * Interrupt classes
 * Double internal test interrupt
 */

#include "double_internal_test.h"

#include "../../logging/logging.h"
#include "../interrupt.h"
#include "internal_test.h"

double_internal_test_interrupt::double_internal_test_interrupt ()
	: interrupt_t ( interrupt_id_t::DOUBLE_INTERNAL_TEST )
{
}

void double_internal_test_interrupt::process ()
{
	logging::info << "ISR of DOUBLE INTERNAL TEST interrupt started" << logging::log_endl;
	interrupt ( new internal_test_interrupt () );
	logging::info << "ISR of DOUBLE INTERNAL TEST interrupt finished" << logging::log_endl;
}
