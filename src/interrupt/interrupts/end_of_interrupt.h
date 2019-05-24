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
	end_of_interrupt ( int _return_value );

	void set_return_value ( int _return_value );
	int get_return_value () const;

	virtual void process ();

private:
	int return_value;
};
