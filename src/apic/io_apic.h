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

/**
 * class io_apic
 * Each instance of this class simulates an I/O APIC
 * and monitor interrupts from a certain external device
 */
class io_apic
{
public:
	io_apic ();
	io_apic ( int _mother_board_id );
	~io_apic ();

	void enable ();
	void disable ();

	bool is_enabled () const;

	void set_mother_board_id ( int id );
	int get_mother_board_id () const;

	void interrupt ( external_interrupt_t *current_interrupt );

private:
	void send_disable_signal ();

	void io_apic_thread_entry ( status_t father_thread_status );
	void io_apic_thread_event_loop ();

	bool do_events ( external_interrupt_t *current_interrupt );

	int mother_board_id;   // Motherboard ID

	bool enabled;   // is I/O APIC enabled
	std::thread io_apic_thread;   // I/O APIC daemon thread

	thread_safe_queue < external_interrupt_t * > event_queue;   // event queue
};

extern io_apic *io_apic_list;   // List of I/O apics

void init_io_apic ( int n );
void destroy_io_apic ();
