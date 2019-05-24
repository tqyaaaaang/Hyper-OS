/**
 * core/core.h
 * CPU Core
 */

#pragma once

#include <mutex>
#include "../mmu/mmu.h"
#include "../apic/local_apic.h"
#include "../context/context.h"
#include "../process/process_t.h"
#include "../process/process.h"

class CPU_mmu;

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

	/**
	 * CPU_core.inc_interrupt_depth ()
	 * increase the depth of the interrupt stack
	 */
	void inc_interrupt_depth ();

	/**
	 * CPU_core.dec_interrupt_depth ()
	 * decrease the depth of the interrupt stack
	 */
	void dec_interrupt_depth ();

	/**
	 * CPU_core.get_interrupt_depth ()
	 * get the depth of the interrupt stack
	 */
	int get_interrupt_depth () const;

	/**
	 * CPU_core.set_interrupt_flag
	 * Set interrupt waiting flag
	 * Process get mark and yeild in tail-check
	 */
	void set_interrupt_waiting_flag();

	/**
	 * CPU_core.unset_interrupt_flag
	 * Unet interrupt waiting flag
	 */
	void unset_interrupt_waiting_flag();

	/**
	 * CPU_core.get_interrupt_flag
	 * Get interrupt waiting flag
	 */
	int get_interrupt_waiting_flag() const;

	
	void set_core_id ( int id );   // Set the CPU core id
	int get_core_id () const;   // Get the CPU core id

	local_apic & get_lapic ();   // Get the Loacl APIC of this core

	void acquire ();   // Acquire the ownership of this CPU
	void release ();   // Release the ownership of this CPU

	std::mutex & get_cpu_lock ();   // Get CPU lock mutex

	void set_context(const context_t &context);
	context_t get_context() const;

	void set_current(process_t *current);
	process_t* get_current() const;
	
	void vm_read(char *buf, size_t la_begin, size_t la_end);
	void vm_write(size_t addr, const char *buf_begin, const char *buf_end);	
	
private:
	bool enabled_flag;
	bool interrupt_enabled_flag;
	bool _preserved_interrupt_enabled_flag;
	int core_id;

	int interrupt_depth;

	local_apic lapic;
	
	std::mutex cpu_lock;

	CPU_mmu mmu;
	context_t context;
	process_t *current;

	int interrupt_waiting_flag;
	mutable std::mutex interrupt_waiting_flag_mutex;
};
