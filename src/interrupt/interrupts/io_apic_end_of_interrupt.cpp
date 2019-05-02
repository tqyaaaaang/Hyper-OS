/**
 * interrupt/interrupts/io_apic_end_of_interrupt.cpp
 * Interrupt classes
 * End of Interrupt signal to I/O APIC
 */

#include "io_apic_end_of_interrupt.h"
#include "../interrupt_id.h"

io_apic_end_of_interrupt::io_apic_end_of_interrupt ()
	: io_apic_signal_t ( interrupt_id_t::IO_APIC_END_OF_INTERRUPT )
{
}

void io_apic_end_of_interrupt::process ()
{
}
