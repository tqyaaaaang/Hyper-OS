/**
 * apic/io_apic.h
 * I/O APIC
 */

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include "../utils/thread_safe_queue/thread_safe_queue.h"

class external_interrupt_t;
class status_t;
class motherboard_t;

/**
 * class io_apic
 * Each instance of this class simulates an I/O APIC
 * and monitor interrupts from a certain external device
 */
class io_apic
{
public:
	io_apic ();
	io_apic ( motherboard_t *_motherboard );
	~io_apic ();

	void enable ();
	void disable ();

	bool is_enabled () const;

	void set_motherboard ( motherboard_t *_motherboard );
	motherboard_t * get_motherboard () const;

	void interrupt ( external_interrupt_t *current_interrupt );

private:
	void send_disable_signal ();

	void io_apic_thread_entry ( status_t father_thread_status );
	void io_apic_thread_event_loop ();

	bool do_events ( external_interrupt_t *current_interrupt );

	motherboard_t *motherboard;   // Motherboard

	bool enabled;   // is I/O APIC enabled
	std::thread io_apic_thread;   // I/O APIC daemon thread

	thread_safe_queue < external_interrupt_t * > event_queue;   // event queue
};

void init_io_apic ();
