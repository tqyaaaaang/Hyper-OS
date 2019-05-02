/**
 * interrupt/interrupts/clock_interrupt.h
 * Interrupt classes
 * Clock interrupt
 */

#pragma once

#include "external_interrupt_t.h"

class clock_interrupt : public external_interrupt_t
{
public:
	clock_interrupt ();

	virtual void process ();
};
