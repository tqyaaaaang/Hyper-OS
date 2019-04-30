/**
 * core/core.h
 * CPU Core
 */

#pragma once

#include <mutex>
#include "../apic/local_apic.h"

/**
 * class CPU_core
 * Each instance of this class is an unique cpu core
 * member function start ()
 */
class CPU_core
{
public:
	CPU_core ();
	~CPU_core ();

	/**
	* CPU_core.set_enabled
	* set the CPU status
	*/
	void set_enabled ( bool status );

	/**
	 * CPU_core.enable
	 * enable the CPU
	 */
	void enable ();

	/**
	 * CPU_core.disable
	 * disable the CPU
	 */
	void disable ();

	/**
	 * CPU_core.is_enabled
	 * check if the core is enabled
	 */
	bool is_enabled () const;

	/**
	 * CPU_core.set_interrupt_enabled
	 * set the CPU interrupt status
	 */
	void set_interrupt_enabled ( bool status );

	/**
	 * CPU_core.enable_interrupt
	 * enable interrupt
	 */
	void enable_interrupt ();

	/**
	 * CPU_core.disable_interrupt
	 * disable interrupt
	 */
	void disable_interrupt ();

	/**
	 * CPU_core.is_interrupt_enabled
	 * check if the interrupt is enabled
	 */
	bool is_interrupt_enabled () const;

	void set_core_id ( int id );   // Set the CPU core id
	int get_core_id () const;   // Get the CPU core id

	local_apic & get_lapic ();   // Get the Loacl APIC of this core

	void acquire ();   // Acquire the ownership of this CPU
	void release ();   // Release the ownership of this CPU

	std::mutex & get_cpu_lock ();   // Get CPU lock mutex

private:
	bool enabled_flag;
	bool interrupt_enabled_flag;
	int core_id;

	local_apic lapic;

	std::mutex cpu_lock;
};
