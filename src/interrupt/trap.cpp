/**
 * interrupt/trap.cpp
 * Interrupt Service Routine
 */

#include "trap.h"

#include <sstream>
#include <string>
#include "interrupts/interrupt_t.h"
#include "../logging/logging.h"
#include "../core/core.h"
#include "../process/process_t.h"
#include "../schedule/schedule.h"
#include "../message/message.h"
#include "../process/idle.h"
#include <ctime>

using std::string;

/**
 * schedule process
 * RR
 */
static void trap_exit()
{
	try {
		schedule(status.get_core()->get_core_id());
		process_t *proc = status.get_core()->get_current();
		if ((proc == nullptr || proc->get_prog () == nullptr) && !TEST) {   // is nullptr or is idle
			logging::debug << "No process need to run, signal idle to run" << logging::log_endl;
			signal_idle();
		}
	} catch(int id) {
		logging::info << "idle exit with id : " << id << logging::log_endl;
	}
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
	
	message::interrupt ( message::wrap_core_info ( "kern trap" ) ) << "(switch to kernel mode) trap entry of interrupt : " << current_interrupt->to_string () << message::msg_endl;
	logging::debug << "Interrupt Service Routine for CPU #" << status.get_core ()->get_core_id () << " created" << logging::log_endl;

	logging::debug << "CPU #" << status.get_core ()->get_core_id () << " received interrupt : " << current_interrupt->to_string () << logging::log_endl;

	process_t *cur = status.get_core()->get_current();
	
	status.get_core()->acquire();	
	status.get_core ()->inc_interrupt_depth ();

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

	message::interrupt ( message::wrap_core_info ( "kern trap" ) ) << "(switch to user mode) trap exit, restore context of current process" << message::msg_endl;

	status.get_core ()->dec_interrupt_depth ();
	status.get_core()->release ();

	status.get_core ()->get_lapic ().send_end_of_interrupt ( 0 );

	clock_t e = clock();
	logging::debug << "interrupt : " << 1.0*(e-c)/CLOCKS_PER_SEC << logging::log_endl;
}

