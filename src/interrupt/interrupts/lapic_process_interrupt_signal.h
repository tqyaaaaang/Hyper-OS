/**
 * interrupt/interrupts/lapic_process_interrupt_signal.h
 * Interrupt classes
 * Signal to let Local APIC send a new interrupt
 */

#pragma once

#include "lapic_signal_t.h"

class lapic_process_interrupt_signal : public lapic_signal_t
{
public:
	lapic_process_interrupt_signal ();

	virtual void process ();
};
