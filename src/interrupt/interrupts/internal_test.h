/**
 * interrupt/interrupts/internal_test.h
 * Interrupt classes
 * Internal test interrupt
 */

#pragma once

#include "interrupt_t.h"

class internal_test_interrupt : public interrupt_t
{
public:
	internal_test_interrupt ();

	virtual void process ();
};
