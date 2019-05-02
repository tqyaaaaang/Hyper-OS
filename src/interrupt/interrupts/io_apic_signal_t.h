/**
 * interrupt/interrupts/io_apic_signal_t.h
 * Interrupt classes
 * Abstract base class io_apic_signal_t
 * I/O APIC signals
 */

#pragma once

#include "external_interrupt_t.h"

class io_apic_signal_t : public external_interrupt_t
{
public:
	io_apic_signal_t ( interrupt_id_t interrupt_id );

	virtual bool is_io_apic_signal () const;
};
