/**
 * interrupt/interrupts/disable_lapic.h
 * Interrupt classes
 * Disable the LAPIC
 */

#pragma once

#include "interrupt_t.h"

class disable_lapic : public interrupt_t
{
public:
	disable_lapic ();

	virtual void process ();
};
