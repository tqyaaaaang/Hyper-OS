/**
 * interrupt/interrupts/end_of_interrupt.cpp
 * Interrupt classes
 * End of Interrupt signal to Local APIC
 */

#include "end_of_interrupt.h"

end_of_interrupt::end_of_interrupt ()
	: interrupt_t ( interrupt_id_t::END_OF_INTERRUPT )
{
}

void end_of_interrupt::process ()
{
}
