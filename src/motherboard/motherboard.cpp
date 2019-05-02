/**
 * motherboard/motherboard.cpp
 * Motherboard
 */

#include "motherboard.h"
#include "../logging/logging.h"

motherboard_t *motherboards;   // List of motherboards

motherboard_t::motherboard_t ()
	: ioapic ( this )
{
}

motherboard_t::~motherboard_t ()
{
}

void motherboard_t::set_id ( int _id )
{
}

int motherboard_t::get_id () const
{
	return 0;
}

io_apic & motherboard_t::get_io_apic ()
{
	return ioapic;
}

void motherboard_t::send_interrupt ( external_interrupt_t * current_interrupt )
{
	get_io_apic ().interrupt ( current_interrupt );
}



void init_motherboards ( int n )
{
	logging::info << "Initializing " << n << " motherboards" << logging::log_endl;
	motherboards = new motherboard_t[n];
	for ( int i = 0; i < n; ++i ) {
		logging::debug << "Initializing motherboard #" << i << logging::log_endl;
		motherboards[i].set_id ( i );
		motherboards[i].get_io_apic ().enable ();
	}
}

void destroy_motherboards ()
{
	logging::debug << "Destroying motherboards" << logging::log_endl;

	delete[] motherboards;
}
