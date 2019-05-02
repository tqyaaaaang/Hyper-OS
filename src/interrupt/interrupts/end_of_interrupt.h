/**
 * interrupt/interrupts/end_of_interrupt.h
 * Interrupt classes
 * End of Interrupt signal to Local APIC
 */

#pragma once

#include "lapic_signal_t.h"

class end_of_interrupt : public lapic_signal_t
{
public:
	end_of_interrupt ();

	virtual void process ();
};
