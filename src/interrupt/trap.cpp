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

	status.get_core ()->inc_interrupt_depth ();

	status.get_core ()->set_interrupt_waiting_flag ();
	process_t *cur = status.get_core()->get_current();
	
	status.get_core()->acquire();	
	if (cur != nullptr) {
		//make sure current proc slept successfully
		cur->cond_mutex.lock();
		cur->cond_mutex.unlock();
		logging::debug << "CPU #" << status.get_core()->get_core_id() << " trap into kernel mode of " << cur->get_name() << logging::log_endl;
	    
	} else {
		logging::debug << "CPU #" << status.get_core()->get_core_id() << " trap into kernel mode of idle" << logging::log_endl;
	}
	if (cur != nullptr) {
		logging::info << "trap of " << cur->get_name() << " " << cur->get_pid() << " start process." << logging::log_endl;
	}
	current_interrupt->process ();
	trap_exit();
	status.get_core()->release ();
	status.get_core ()->unset_interrupt_waiting_flag();
	
	status.get_core ()->get_lapic ().send_end_of_interrupt ( 0 );

	status.get_core ()->dec_interrupt_depth ();

	clock_t e = clock();
	logging::debug << "interrupt : " << 1.0*(e-c)/CLOCKS_PER_SEC << logging::log_endl;
}

