/**
 * apic/io_apic.cpp
 * I/O APIC
 */

#include "io_apic.h"
#include "interrupt_affinity.h"
#include "../logging/logging.h"
#include "../status/status.h"
#include "../interrupt/interrupts/disable_io_apic.h"
#include "../interrupt/interrupts/io_apic_end_of_interrupt.h"
#include "interrupt_affinity.h"
#include "../core/cpus.h"
#include "../env/env.h"
#include <sstream>



io_apic *io_apic_list;   // List of I/O APIC

io_apic::io_apic ()
{
}

io_apic::io_apic ( int _mother_board_id )
	: mother_board_id ( _mother_board_id )
{
}

io_apic::~io_apic ()
{
	send_disable_signal ();
	io_apic_thread.join ();
}

void io_apic::enable ()
{
	logging::debug << "Enabling I/O APIC for Motherboard #" << get_mother_board_id () << logging::log_endl;
	enabled = true;
	io_apic_thread = std::thread ( &io_apic::io_apic_thread_entry, this, status );
}

void io_apic::disable ()
{
	logging::debug << "Disabling I/O APIC for Motherboard #" << get_mother_board_id () << logging::log_endl;
	enabled = false;
	send_disable_signal ();
	io_apic_thread.join ();

	event_queue.clear ();
}

bool io_apic::is_enabled () const
{
	return enabled;
}

void io_apic::set_mother_board_id ( int id )
{
	mother_board_id = id;
}

int io_apic::get_mother_board_id () const
{
	return mother_board_id;
}

void io_apic::interrupt ( external_interrupt_t * current_interrupt )
{
	logging::debug << "I/O APIC received interrupt request : " << current_interrupt->to_string () << logging::log_endl;
	event_queue.push_back ( current_interrupt );
}

void io_apic::send_disable_signal ()
{
	interrupt ( new disable_io_apic () );
}

void io_apic::io_apic_thread_entry ( status_t father_thread_status )
{
	status = father_thread_status;
	status.set_core ( NULL );
	std::stringstream string_helper;
	string_helper << "I/O APIC of Motherboard #" << get_mother_board_id ();
	status.set_name ( string_helper.str () );
	logging::debug << "I/O APIC for Motherboard #" << get_mother_board_id () << " created" << logging::log_endl;

	io_apic_thread_event_loop ();
}

void io_apic::io_apic_thread_event_loop ()
{
	while ( true ) {
		external_interrupt_t *current_interrupt = event_queue.pop_front ();

		if ( current_interrupt->is_io_apic_signal () ) {   // signals sent to I/O APIC
			if ( do_events ( current_interrupt ) ) {
				return;
			}
		} else {   // interrupts
			logging::debug << "I/O APIC received new interrupt request : " << current_interrupt->to_string () << logging::log_endl;
			CPU_core &current_core = cores[get_interrupt_affinity ( current_interrupt->get_interrupt_id () )];
			current_core.get_lapic ().interrupt ( current_interrupt, false );
		}
	}
}

bool io_apic::do_events ( external_interrupt_t * current_interrupt )
{
	logging::debug << "I/O APIC received signal : " << current_interrupt->to_string () << logging::log_endl;
	switch ( current_interrupt->get_interrupt_id () ) {
	case interrupt_id_t::DISABLE_IO_APIC:
		logging::debug << "I/O APIC received DISABLE signal" << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( 0 );
		return true;
		break;

	default:
		logging::error << "I/O APIC do_events : unknown event id " << static_cast < int > ( current_interrupt->get_interrupt_id () ) << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( 0 );
		break;
	}
	return false;
}



void init_io_apic ( int n )
{
	init_interrupt_affinity ();

	logging::info << "Initializing I/O " << n << " APICs" << logging::log_endl;
	io_apic_list = new io_apic[n];
	for ( int i = 0; i < n; ++i ) {
		logging::debug << "Initializing I/O APIC #" << i << logging::log_endl;
		io_apic_list[i].set_mother_board_id ( i );
		io_apic_list[i].enable ();
	}
}

void destroy_io_apic ()
{
	logging::debug << "Destroying I/O APICs" << logging::log_endl;

	delete[] io_apic_list;
}
