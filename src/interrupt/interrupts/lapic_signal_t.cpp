/**
 * interrupt/interrupts/lapic_signal_t.cpp
 * Interrupt classes
 * Abstract base class lapic_signal_t
 * LAPIC signals
 */

#include "lapic_signal_t.h"

lapic_signal_t::lapic_signal_t ( interrupt_id_t interrupt_id )
	: interrupt_t ( interrupt_id )
{
}

bool lapic_signal_t::is_lapic_signal () const
{
	return true;
}
