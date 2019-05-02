/**
 * interrupt/interrupts/io_apic_end_of_interrupt.h
 * Interrupt classes
 * End of Interrupt signal to I/O APIC
 */

#pragma once

#include "io_apic_signal_t.h"

class io_apic_end_of_interrupt : public io_apic_signal_t
{
public:
	io_apic_end_of_interrupt ();

	virtual void process ();

private:
	external_interrupt_t * current_interrupt;
	int return_value;
};
