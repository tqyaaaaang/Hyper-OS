/**
 * interrupt/interrupts/end_of_interrupt.cpp
 * Interrupt classes
 * End of Interrupt signal to Local APIC
 */

#include "end_of_interrupt.h"

end_of_interrupt::end_of_interrupt ( int _return_value )
	: lapic_signal_t ( interrupt_id_t::END_OF_INTERRUPT )
	, return_value ( _return_value )
{
}

 void end_of_interrupt::set_return_value ( int _return_value )
 {
	 return_value = _return_value;
 }

 int end_of_interrupt::get_return_value () const
 {
	 return return_value;
 }

 void end_of_interrupt::process ()
{
}
