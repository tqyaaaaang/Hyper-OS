/**
 * interrupt/interrupts/keyboard_interrupt.h
 * Interrupt classes
 * Keyboard interrupt
 */

#pragma once

#include "external_interrupt_t.h"

class keyboard_interrupt : public external_interrupt_t
{
public:
	keyboard_interrupt ( std::string _data );

	virtual void process ();

private:
	void signal_data ( int data );

	std::string data;
};
