/**
 * interrupt/trap.cpp
 * Interrupt Service Routine
 */

#include "trap.h"

#include <sstream>
#include "interrupts/interrupt_t.h"
#include "../logging/logging.h"
#include "../core/core.h"

void interrupt_trap_entry ( status_t thread_status, interrupt_t * current_interrupt )
{
	status = thread_status;
	logging::debug << "Interrupt Service Routine for CPU #" << status.get_core ()->get_core_id () << " created" << logging::log_endl;

	logging::info << "CPU #" << status.get_core ()->get_core_id () << " received interrupt : " << current_interrupt->to_string () << logging::log_endl;

	status.get_core ()->acquire ();
	current_interrupt->process ();
	status.get_core ()->release ();

	status.get_core ()->get_lapic ().send_end_of_interrupt ();
}
