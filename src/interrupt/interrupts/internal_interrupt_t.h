/**
 * interrupt/interrupts/internal_interrupt_t.h
 * Interrupt classes
 * Abstract base class internal_interrupt_t
 * Internal interrupts
 */

#pragma once

#include "interrupt_t.h"

class internal_interrupt_t : public interrupt_t
{
public:
	internal_interrupt_t ( interrupt_id_t interrupt_id );

	virtual bool is_internal_interrupt () const;
};
