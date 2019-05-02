/**
 * interrupt/interrupts/disable_io_apic.cpp
 * Interrupt classes
 * Disable the I/O APIC
 */

#include "disable_io_apic.h"

disable_io_apic::disable_io_apic ()
	: io_apic_signal_t ( interrupt_id_t::DISABLE_IO_APIC )
{
}

void disable_io_apic::process ()
{
}
