/**
 * interrupt/interrupts/disable_io_apic.h
 * Interrupt classes
 * Disable the I/O APIC
 */

#pragma once

#include "io_apic_signal_t.h"

class disable_io_apic : public io_apic_signal_t
{
public:
	disable_io_apic ();

	virtual void process ();
};
