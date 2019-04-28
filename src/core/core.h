/**
 * core/core.h
 * CPU Core
 */

#pragma once

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

private:
	bool enabled_flag;
	bool interrupt_enabled_flag;
	int core_id;
};