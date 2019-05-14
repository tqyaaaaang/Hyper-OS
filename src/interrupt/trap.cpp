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
#include <ctime>

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

	clock_t c = clock();
	status = thread_status;
	logging::debug << "Interrupt Service Routine for CPU #" << status.get_core ()->get_core_id () << " created" << logging::log_endl;

	logging::debug << "CPU #" << status.get_core ()->get_core_id () << " received interrupt : " << current_interrupt->to_string () << logging::log_endl;

	status.get_core ()->mark_intr ();
	process_t *cur = status.get_core()->get_current();

	if (cur != nullptr) {
		//make sure current proc slept successfully

		status.get_core ()->acquire ();
		
		cur->cond_mutex.lock();

		logging::debug << "CPU #" << status.get_core()->get_core_id() << " trap into kernel mode of " << cur->get_name()  << logging::log_endl;
		
		cur->cond_mutex.unlock();
		
		current_interrupt->process ();
		status.get_core ()->unmark_intr();
		trap_exit();
		status.get_core ()->release ();
	}
	
	status.get_core ()->get_lapic ().send_end_of_interrupt ();
	clock_t e = clock();
	logging::debug << "interrupt : " << 1.0*(e-c)/CLOCKS_PER_SEC << logging::log_endl;
}

