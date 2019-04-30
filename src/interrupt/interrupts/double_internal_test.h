/**
 * interrupt/interrupts/double_internal_test.h
 * Interrupt classes
 * Double internal test interrupt
 */

#pragma once

#include "interrupt_t.h"

class double_internal_test_interrupt : public interrupt_t
{
public:
	double_internal_test_interrupt ();

	virtual void process ();
};
