/**
 * interrupt/interrupts/internal_interrupt_t.cpp
 * Interrupt classes
 * Abstract base class internal_interrupt_t
 * Internal interrupts
 */

#include "internal_interrupt_t.h"

internal_interrupt_t::internal_interrupt_t ( interrupt_id_t interrupt_id )
	: interrupt_t ( interrupt_id )
{
}

bool internal_interrupt_t::is_internal_interrupt () const
{
	return true;
}
