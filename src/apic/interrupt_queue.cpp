/**
 * apic/interrupt_queue.h
 * Queue of interrupts
 */

#include "interrupt_queue.h"
#include "../interrupt/interrupts/interrupt_t.h"

interrupt_queue_t::interrupt_queue_t ()
{
}

interrupt_queue_t::~interrupt_queue_t ()
{
}

void interrupt_queue_t::push ( interrupt_t * current_interrupt )
{
	++time_id;
	int interrupt_type;
	if ( current_interrupt->is_internal_interrupt () ) {
		interrupt_type = 0;
	} else {
		interrupt_type = 1;
	}
	interrupt_queue.push ( std::make_pair ( std::make_pair ( interrupt_type, std::make_pair ( static_cast < int > ( current_interrupt->get_interrupt_id () ) >> 4, time_id ) ), current_interrupt ) );
}

interrupt_t * interrupt_queue_t::front () const
{
	return interrupt_queue.top ().second;
}

void interrupt_queue_t::pop ()
{
	interrupt_queue.pop ();
}

bool interrupt_queue_t::empty () const
{
	return interrupt_queue.empty ();
}

void interrupt_queue_t::clear ()
{
	while ( !empty () ) {
		pop ();
	}
}
