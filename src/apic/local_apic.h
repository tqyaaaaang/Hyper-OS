/**
 * apic/local_apic.h
 * Local APIC
 */

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <queue>
#include "interrupt_queue.h"
#include "../utils/thread_safe_queue/thread_safe_queue.h"

class CPU_core;
class status_t;
class interrupt_t;

/**
 * class local_apic
 * Each instance of this class simulates an Local APIC
 * and send interrupts to a certain CPU core
 */
class local_apic
{
public:
	local_apic ( CPU_core *_core );
	~local_apic ();

	void enable ();   // Enable interrupt
	void disable ();   // Disable interrupt

	bool is_enabled () const;

	int interrupt ( interrupt_t *current_interrupt, bool blocked = true );   // @return: whether the interrupt is processed
	void send_end_of_interrupt ( int return_value );   // send an end_of_interrupt signal

	void try_process_interrupt ();   // check if there is a interrupt waiting and give up current CPU to process it if yes

private:
	int wait_interrupt_return ( interrupt_t *current_interrupt );

	void send_process_interrupt_signal ();   // send a signal to LAPIC to try to process a new interrupt
	void send_disable_signal ();   // send disable LAPIC signal

	void lapic_thread_entry ( status_t father_thread_status );   // LAPIC daemon thread entry
	void lapic_thread_event_loop ();

	bool do_events ( interrupt_t *current_interrupt );   // return true to stop the LAPIC thread
	void schedule ( bool internal_only );   // choose a new interrupt to run
	void run_isr ( interrupt_t *current_interrupt );   // run ISR of an interrupt

	CPU_core *core;   // CPU core in which it affliate
	bool enabled;   // is LAPIC enabled
	std::thread lapic_thread;   // LAPIC daemon thread

	std::stack < std::pair < interrupt_t *, std::thread > > isr_stack;   // stack of current running ISRs
	interrupt_queue_t interrupt_queue;   // interrupts waiting
	thread_safe_queue < interrupt_t * > event_queue;   // event queue
};
