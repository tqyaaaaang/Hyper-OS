/**
 * apic/local_apic.h
 * Local APIC
 */

#pragma once

#include <thread>

class CPU_core;

class status_t;

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

private:
	void lapic_thread_entry ( status_t father_thread_status );

	bool enabled;
	std::thread lapic_thread;
	CPU_core *core;
};
