/**
 * apic/local_apic.cpp
 * Local APIC
 */


#include "local_apic.h"

#include <sstream>

#include "../core/core.h"
#include "../logging/logging.h"
#include "../status/status.h"
#include "../interrupt/interrupts/interrupt_t.h"
#include "../interrupt/interrupts/end_of_interrupt.h"
#include "../interrupt/interrupts/disable_lapic.h"
#include "../interrupt/trap.h"

local_apic::local_apic ( CPU_core *_core )
	: core ( _core )
{
}

local_apic::~local_apic ()
{
	if ( enabled ) {
		enabled = false;
		send_disable_signal ();
		lapic_thread.join ();
	}
}

void local_apic::enable ()
{
	logging::debug << "Enabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
	enabled = true;
	lapic_thread = std::thread ( &local_apic::lapic_thread_entry, this, status );
}

void local_apic::disable ()
{
	logging::debug << "Enabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
	enabled = false;
	lapic_thread.join ();

	while ( !isr_stack.empty () ) {
		isr_stack.pop ();
	}
	while ( !interrupt_queue.empty () ) {
		interrupt_queue.pop ();
	}
	event_queue.clear ();
}

bool local_apic::is_enabled () const
{
	return enabled;
}

/**
 * send an interrupt
 * @current_interrupt: current interrupt descriptor
 * @return: whether the interrupt is processed
 *          0: ok
 *         -1: error because LAPIC is not enabled
 */
int local_apic::interrupt ( interrupt_t *current_interrupt )
{
	if ( interrupt_id_is_internal_exception ( current_interrupt->get_interrupt_id () ) && !is_enabled () ) {
		current_interrupt->get_return_promise ().set_value ( -1 );
		return -1;
	}
	event_queue.push_back ( current_interrupt );
	if ( !interrupt_id_is_signal ( current_interrupt->get_interrupt_id () ) ) {
		core->release ();
	}
	int return_val = current_interrupt->get_return_promise ().get_future ().get ();
	if ( !interrupt_id_is_signal ( current_interrupt->get_interrupt_id () ) ) {
		core->acquire ();
	}
	return return_val;
}

void local_apic::send_end_of_interrupt ()
{
	interrupt ( new end_of_interrupt () );
}



void local_apic::send_disable_signal ()
{
	interrupt ( new disable_lapic () );
}

void local_apic::lapic_thread_entry ( status_t father_thread_status )
{
	status = father_thread_status;
	std::stringstream string_helper;
	string_helper << "LAPIC of CPU #" << core->get_core_id ();
	status.set_name ( string_helper.str () );
	logging::debug << "Local APIC for CPU #" << core->get_core_id () << " created" << logging::log_endl;

	lapic_thread_event_loop ();
}

void local_apic::lapic_thread_event_loop ()
{
	while ( true ) {
		interrupt_t *current_interrupt = event_queue.pop_front ();

		if ( interrupt_id_is_signal ( current_interrupt->get_interrupt_id () ) ) {   // signals sent to LAPIC
			if ( do_events ( current_interrupt ) ) {
				return;
			}
		} else if ( interrupt_id_is_internal_exception ( current_interrupt->get_interrupt_id () ) ) {   // internal CPU exceptions
			logging::debug << "LAPIC received new interrupt request : " << current_interrupt->to_string () << logging::log_endl;
			run_isr ( current_interrupt );
		} else {   // hardware interrupts
			logging::debug << "LAPIC received new interrupt request : " << current_interrupt->to_string () << logging::log_endl;
			interrupt_queue.push ( current_interrupt );
		}
	}
}

/**
 * do_events
 * process signals snet to LAPIC
 * @return: the status of the LAPIC
 *          true:  exit LAPIC thread
 *          false: kill running
 */
bool local_apic::do_events ( interrupt_t * current_interrupt )
{
	logging::debug << "LAPIC received signal : " << current_interrupt->to_string () << logging::log_endl;
	switch ( current_interrupt->get_interrupt_id () ) {
	case interrupt_id_t::END_OF_INTERRUPT:
		logging::debug << "LAPIC received EOI signal from ISR : " << isr_stack.top ().first->to_string () << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( 0 );
		isr_stack.top ().second.join ();
		isr_stack.top ().first->get_return_promise ().set_value ( 0 );
		isr_stack.pop ();
		if ( isr_stack.empty () ) {
			if ( !is_enabled () ) {   // LAPIC disabled
				return true;
			}
			schedule ();
		}
		break;

	case interrupt_id_t::DISABLE_LAPIC:
		logging::debug << "LAPIC received DISABLE signal" << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( 0 );
		if ( isr_stack.empty () ) {
			if ( !is_enabled () ) {
				return true;
			}
		}
		break;

	default:
		logging::error << "LAPIC do_events : unknown event id " << static_cast < int > ( current_interrupt->get_interrupt_id () ) << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( 0 );
		break;
	}
	return false;
}

void local_apic::schedule ()
{
	logging::debug << "LAPIC scheduling new ISR to run" << logging::log_endl;
	if ( core->is_interrupt_enabled () && !interrupt_queue.empty () ) {
		interrupt_t *current_interrupt = interrupt_queue.front ();
		interrupt_queue.pop ();
		logging::debug << "LAPIC scheduled interrupt : " << current_interrupt->to_string () << logging::log_endl;
		run_isr ( current_interrupt );
	} else if ( !core->is_interrupt_enabled () ) {
		logging::debug << "LAPIC schedule failed because interrupt is disabled" << logging::log_endl;
	} else {
		logging::debug << "LAPIC schedule failed because no interrupts are waiting" << logging::log_endl;
	}
}

void local_apic::run_isr ( interrupt_t * current_interrupt )
{
	logging::debug << "Calling ISR of interrupt : " << current_interrupt->to_string () << logging::log_endl;
	status_t isr_status = status;
	isr_status.set_core ( core );
	std::stringstream string_helper;
	string_helper << "ISR #" << isr_stack.size () << " of CPU #" << core->get_core_id ();
	isr_status.set_name ( string_helper.str () );
	isr_stack.push ( std::make_pair ( current_interrupt, std::thread ( interrupt_trap_entry, isr_status, current_interrupt ) ) );
}
