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

private:
	bool enabled_flag;
	bool interrupt_enabled_flag;
};