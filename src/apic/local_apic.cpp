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
#include "../interrupt/interrupts/lapic_process_interrupt_signal.h"
#include "../interrupt/trap.h"

local_apic::local_apic ( CPU_core *_core )
	: core ( _core )
	, enabled ( false )
{
}

local_apic::~local_apic ()
{
	if ( enabled ) {
		enabled = false;
		send_disable_signal ();
		lapic_thread.join ();
		while ( !interrupt_queue.empty () ) {
			if ( interrupt_queue.front ()->is_lapic_signal () ) {
				delete interrupt_queue.front ();
			} else {
				interrupt_queue.front ()->get_return_promise ().set_value ( -1 );
			}
			interrupt_queue.pop ();
		}
	}
}

void local_apic::enable ()
{
	if ( !is_enabled () ) {
		logging::debug << "Enabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
		enabled = true;
		lapic_thread = std::thread ( &local_apic::lapic_thread_entry, this, status );
	} else {
		logging::debug << "Trying to enable local APIC for CPU #" << core->get_core_id () << ", but failed because it is already enabled" << logging::log_endl;
	}
}

void local_apic::disable ()
{
	if ( is_enabled () ) {
		logging::debug << "Disabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
		enabled = false;
		send_disable_signal ();
		lapic_thread.join ();

		while ( !isr_stack.empty () ) {
			isr_stack.pop ();
		}
		while ( !interrupt_queue.empty () ) {
			if ( interrupt_queue.front ()->is_lapic_signal () ) {
				delete interrupt_queue.front ();
			} else {
				interrupt_queue.front ()->get_return_promise ().set_value ( -1 );
			}
			interrupt_queue.pop ();
		}
		event_queue.clear ();
	} else {
		logging::debug << "Trying to disable local APIC for CPU #" << core->get_core_id () << ", but failed because it is already disabled" << logging::log_endl;
	}
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
 *         -2: need sleep process
 */
int local_apic::interrupt ( interrupt_t *current_interrupt, bool blocked )
{
	logging::debug << "LAPIC received interrupt request : " << current_interrupt->to_string () << logging::log_endl;
	if ( !current_interrupt->is_lapic_signal () && !is_enabled () ) {
		logging::warning << "LAPIC received interrupt request after it is disabled : " << current_interrupt->to_string () << logging::log_endl;
		current_interrupt->get_return_promise ().set_value ( -1 );
		return -1;
	}
	process_t *proc = core->get_current();
	event_queue.push_back ( current_interrupt );
	int return_val = 0;
	if ( blocked ) {
		if ( core->get_interrupt_depth () == 0 && core->get_current () != nullptr ) {
			std::unique_lock < std::mutex > lck ( core->get_current ()->cond_mutex );
			core->release ();
			logging::info << "process " << core->get_current()->get_name() << " " << core->get_current()->get_pid() << " stop and wait for interrupt" << logging::log_endl;
			core->get_current ()->cond_var.wait ( lck );
			if (proc->get_exit_flag()) {
				throw 0;
			}
		} else {
			core->release ();
		}
		return_val = wait_interrupt_return ( current_interrupt );
		core->acquire ();
	} else {
		std::thread wait_thread ( &local_apic::wait_interrupt_return, this, current_interrupt );
		wait_thread.detach ();
	}
	return return_val;
}

void local_apic::send_end_of_interrupt ( int return_value )
{
	interrupt ( new end_of_interrupt ( return_value ), false );
}

void local_apic::try_process_interrupt ()
{
	logging::debug << "Try to send signals to LAPIC process interrupts if interrupts exist, proccess #"
		<< status.get_core ()->get_current ()->get_pid () << " ( " << status.get_core ()->get_current ()->get_name () << " )" << logging::log_endl;
	process_t *proc = status.get_core()->get_current();
	while ( status.get_core ()->get_interrupt_waiting_flag () ) {
		logging::debug << "Find unprocessed interrupt, sending signal to LAPIC" << logging::log_endl;
		std::unique_lock < std::mutex > lck ( status.get_core ()->get_current ()->cond_mutex );
		status.get_core ()->release ();
		send_process_interrupt_signal ();
		status.get_core ()->get_current ()->cond_var.wait ( lck );
		if (proc->get_exit_flag()) {
			throw 0;
		}
		status.get_core ()->acquire ();
	}
}



int local_apic::wait_interrupt_return ( interrupt_t * current_interrupt )
{
	int return_value = current_interrupt->get_return_promise ().get_future ().get ();
	delete current_interrupt;
	return return_value;
}



void local_apic::send_process_interrupt_signal ()
{
	interrupt ( new lapic_process_interrupt_signal (), false );
}

void local_apic::send_disable_signal ()
{
	interrupt ( new disable_lapic (), false );
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

		if ( current_interrupt->is_lapic_signal () ) {   // signals sent to LAPIC
			if ( do_events ( current_interrupt ) ) {
				return;
			}
		} else if ( current_interrupt->is_internal_interrupt () ) {   // internal CPU exceptions
			logging::debug << "LAPIC received new interrupt request : " << current_interrupt->to_string () << logging::log_endl;
			if ( isr_stack.empty () || isr_stack.top ().first->is_internal_interrupt () ) {
				run_isr ( current_interrupt );
			} else {
				interrupt_queue.push ( current_interrupt );
				core->set_interrupt_waiting_flag ();
			}
		} else {   // hardware interrupts
			logging::debug << "LAPIC received new interrupt request : " << current_interrupt->to_string () << logging::log_endl;
			interrupt_queue.push ( current_interrupt );
			core->set_interrupt_waiting_flag ();
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
	bool return_value = false;
	switch ( current_interrupt->get_interrupt_id () ) {
	case interrupt_id_t::END_OF_INTERRUPT:
		logging::debug << "LAPIC received EOI signal from ISR : " << isr_stack.top ().first->to_string () << logging::log_endl;
		isr_stack.top ().second.join ();
		isr_stack.top ().first->get_return_promise ().set_value ( dynamic_cast < end_of_interrupt * > ( current_interrupt )->get_return_value () );
		isr_stack.pop ();
		if ( isr_stack.empty () ) {
			if ( !is_enabled () ) {   // LAPIC disabled
				return_value = true;
				break;
			} else {
				schedule ( true );
			}
		}
		break;

	case interrupt_id_t::DISABLE_LAPIC:
		logging::debug << "LAPIC received DISABLE signal" << logging::log_endl;
		if ( isr_stack.empty () ) {
			if ( !is_enabled () ) {
				return_value = true;
				break;
			}
		}
		break;

	case interrupt_id_t::LAPIC_PROCESS_INTERRUPT:
		logging::debug << "LAPIC received PROCESS INTERRUPT signal" << logging::log_endl;
		schedule ( false );
		break;

	default:
		logging::error << "LAPIC do_events : unknown event id " << static_cast < int > ( current_interrupt->get_interrupt_id () ) << logging::log_endl;
		break;
	}
	current_interrupt->get_return_promise ().set_value ( 0 );
	return return_value;
}

void local_apic::schedule ( bool internal_only )
{
	logging::debug << "LAPIC scheduling new ISR to run" << logging::log_endl;
	if ( core->is_interrupt_enabled () && !interrupt_queue.empty () ) {
		interrupt_t *current_interrupt = interrupt_queue.front ();
		if ( internal_only && !current_interrupt->is_internal_interrupt () ) {
			logging::debug << "LAPIC schedule failed because internal_only is true but next interrupt is not internal interrupt" << logging::log_endl;
		} else {
			interrupt_queue.pop ();
			if ( interrupt_queue.empty () ) {
				core->unset_interrupt_waiting_flag ();
			}
			logging::debug << "LAPIC scheduled interrupt : " << current_interrupt->to_string () << logging::log_endl;
			run_isr ( current_interrupt );
		}
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
