/**
 * interrupt/interrupts/lapic_signal_t.h
 * Interrupt classes
 * Abstract base class lapic_signal_t
 * LAPIC signals
 */

#pragma once

#include "interrupt_t.h"

class lapic_signal_t : public interrupt_t
{
public:
	lapic_signal_t ( interrupt_id_t interrupt_id );

	virtual bool is_lapic_signal () const;
};
