#include "external_interrupt_t.h"
/**
 * interrupt/interrupts/external_interrupt_t.cpp
 * Interrupt classes
 * Abstract base class external_interrupt_t
 * External interrupts
 */

#include "external_interrupt_t.h"

external_interrupt_t::external_interrupt_t ( interrupt_id_t interrupt_id )
	: interrupt_t ( interrupt_id )
{
}

bool external_interrupt_t::is_external_interrupt () const
{
	return true;
}
