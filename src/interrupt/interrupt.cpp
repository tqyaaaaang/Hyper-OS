/**
 * interrupt/interrupt.h
 * Interrupt entry
 */

#include "interrupt.h"

#include "interrupts/interrupt_t.h"
#include "../status/status.h"
#include "../core/core.h"
#include "../apic/local_apic.h"

int interrupt ( interrupt_t * current_interrupt )
{
	return status.get_core ()->get_lapic ().interrupt ( current_interrupt );
}
