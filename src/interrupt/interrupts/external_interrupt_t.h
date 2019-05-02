/**
 * interrupt/interrupts/external_interrupt_t.h
 * Interrupt classes
 * Abstract base class external_interrupt_t
 * External interrupts
 */

#pragma once

#include "interrupt_t.h"

class external_interrupt_t : public interrupt_t
{
public:
	external_interrupt_t ( interrupt_id_t interrupt_id );

	virtual bool is_external_interrupt () const;
};
