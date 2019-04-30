/**
 * interrupt/interrupts/disable_lapic.cpp
 * Interrupt classes
 * Disable the LAPIC
 */

#include "disable_lapic.h"

disable_lapic::disable_lapic ()
	: interrupt_t ( interrupt_id_t::DISABLE_LAPIC )
{
}

void disable_lapic::process ()
{
}
