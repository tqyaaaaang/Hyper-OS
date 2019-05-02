/**
 * interrupt/interrupts/io_apic_signal_t.cpp
 * Interrupt classes
 * Abstract base class io_apic_signal_t
 * I/O APIC signals
 */

#include "io_apic_signal_t.h"

io_apic_signal_t::io_apic_signal_t ( interrupt_id_t interrupt_id )
	: external_interrupt_t ( interrupt_id )
{
}

bool io_apic_signal_t::is_io_apic_signal () const
{
	return true;
}
