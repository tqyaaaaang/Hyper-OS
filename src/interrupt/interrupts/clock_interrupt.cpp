/**
 * interrupt/interrupts/clock_interrupt.h
 * Interrupt classes
 * Clock interrupt
 */

#include "clock_interrupt.h"
#include "../../logging/logging.h"

#include <thread>
#include <chrono>

clock_interrupt::clock_interrupt ()
	: external_interrupt_t ( interrupt_id_t::CLOCK_INTERRUPT )
{
}

void clock_interrupt::process ()
{
	logging::debug << "Clock received a tick" << logging::log_endl;
}
