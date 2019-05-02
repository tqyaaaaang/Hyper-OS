/**
 * interrupt/interrupts/disable_lapic.h
 * Interrupt classes
 * Disable the LAPIC
 */

#pragma once

#include "lapic_signal_t.h"

class disable_lapic : public lapic_signal_t
{
public:
	disable_lapic ();

	virtual void process ();
};
