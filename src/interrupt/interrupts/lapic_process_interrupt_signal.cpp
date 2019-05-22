/**
 * interrupt/interrupts/lapic_process_interrupt_signal.cpp
 * Interrupt classes
 * Signal to let Local APIC send a new interrupt
 */

#include "lapic_process_interrupt_signal.h"

lapic_process_interrupt_signal::lapic_process_interrupt_signal ()
	: lapic_signal_t ( interrupt_id_t::LAPIC_PROCESS_INTERRUPT )
{
}

void lapic_process_interrupt_signal::process ()
{
}
