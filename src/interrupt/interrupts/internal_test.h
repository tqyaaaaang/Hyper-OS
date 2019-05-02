/**
 * interrupt/interrupts/internal_test.h
 * Interrupt classes
 * Internal test interrupt
 */

#pragma once

#include "internal_interrupt_t.h"

class internal_test_interrupt : public internal_interrupt_t
{
public:
	internal_test_interrupt ();

	virtual void process ();
};
