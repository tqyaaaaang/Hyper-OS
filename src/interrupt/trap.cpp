/**
 * interrupt/trap.cpp
 * Interrupt Service Routine
 */

#include "trap.h"

#include <sstream>
#include "interrupts/interrupt_t.h"
#include "../logging/logging.h"
#include "../core/core.h"
#include "../process/process_t.h"
#include "../schedule/schedule.h"

/**
 * schedule process
 * RR
 */
static void trap_exit()
{
	schedule(status.get_core()->get_core_id());
}

/**
 * trap entry
 * apic/lpic call this function when receiveing an interrupt
 * running on kernel mode of current process of the interrupt
 */
void interrupt_trap_entry ( status_t thread_status, interrupt_t * current_interrupt )
{
	status = thread_status;
	logging::debug << "Interrupt Service Routine for CPU #" << status.get_core ()->get_core_id () << " created" << logging::log_endl;

	logging::info << "CPU #" << status.get_core ()->get_core_id () << " received interrupt : " << current_interrupt->to_string () << logging::log_endl;

	status.get_core ()->acquire ();
	current_interrupt->process ();
	trap_exit();
	status.get_core ()->release ();

	status.get_core ()->get_lapic ().send_end_of_interrupt ();
}

