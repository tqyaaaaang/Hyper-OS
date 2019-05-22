/**
 * core/core.h
 * CPU Core
 */

#include "core.h"
#include "../logging/logging.h"
#include <cassert>
#include "../process/process_t.h"
#include <mutex>

using std::mutex;
using std::lock_guard;

CPU_core::CPU_core ()
	: enabled_flag ( false )
	, interrupt_enabled_flag ( false )
	, lapic ( this )
	, interrupt_depth ( 0 )
{
	mmu = CPU_mmu(this);
	current = nullptr;
	interrupt_waiting_flag = 0;
}

CPU_core::~CPU_core ()
{
	disable ();
}

void CPU_core::set_enabled ( bool status )
{
	if ( status ) {
		enable ();
	} else {
		disable ();
	}
}

void CPU_core::enable ()
{
	logging::info << "Enabling CPU #" << get_core_id () << logging::log_endl;
	enabled_flag = true;
	lapic.enable ();
}

void CPU_core::disable ()
{
	logging::info << "Disabling CPU #" << get_core_id () << logging::log_endl;
	enabled_flag = false;
	lapic.disable ();
}

bool CPU_core::is_enabled () const
{
	return enabled_flag;
}

void CPU_core::set_interrupt_enabled ( bool status )
{
	if ( status ) {
		enable_interrupt ();
	} else {
		disable_interrupt ();
	}
}

void CPU_core::enable_interrupt ()
{
	logging::debug << "Enabling interrupt of CPU #" << get_core_id () << logging::log_endl;
	interrupt_enabled_flag = true;
}

void CPU_core::disable_interrupt ()
{
	logging::debug << "Disabling interrupt of CPU #" << get_core_id () << logging::log_endl;
	interrupt_enabled_flag = false;
}

bool CPU_core::is_interrupt_enabled () const
{
	return interrupt_enabled_flag;
}

void CPU_core::inc_interrupt_depth ()
{
	++interrupt_depth;
}

void CPU_core::dec_interrupt_depth ()
{
	--interrupt_depth;
}

int CPU_core::get_interrupt_depth () const
{
	return interrupt_depth;
}

void CPU_core::set_core_id ( int id )
{
	core_id = id;
}

int CPU_core::get_core_id () const
{
	return core_id;
}

local_apic & CPU_core::get_lapic ()
{
	return lapic;
}

void CPU_core::acquire ()
{
	logging::debug << "Acquiring CPU access" << logging::log_endl;
	get_cpu_lock ().lock ();
}

void CPU_core::release ()
{
	logging::debug << "Releasing CPU access" << logging::log_endl;
	get_cpu_lock ().unlock ();
}

std::mutex & CPU_core::get_cpu_lock ()
{
	return cpu_lock;
}

context_t CPU_core::get_context() const
{
	return context;
}

void CPU_core::set_context(const context_t &context)
{
	this->context = context;
}

process_t* CPU_core::get_current() const
{
	return current;
}

void CPU_core::set_current(process_t* current)
{
	this->current = current;
}

void CPU_core::vm_read(char *buf, size_t la_begin, size_t la_end)
{
	for (size_t la = la_begin; la != la_end; la++, buf++) {
		*buf = mmu.read(la);
	}
}

void CPU_core::vm_write(size_t addr, const char *buf_begin, const char *buf_end)	
{
	for (const char *p = buf_begin; p != buf_end; p++, addr++) {
		mmu.write(addr, *p);
	}
}

void CPU_core::set_interrupt_waiting_flag()
{
	lock_guard<mutex> lk(interrupt_waiting_flag_mutex);
	interrupt_waiting_flag++;
}

void CPU_core::unset_interrupt_waiting_flag()
{
	lock_guard<mutex> lk(interrupt_waiting_flag_mutex);
	interrupt_waiting_flag--;
}

int CPU_core::get_interrupt_waiting_flag() const
{
	lock_guard<mutex> lk(interrupt_waiting_flag_mutex);
    int d = interrupt_waiting_flag;
	return d;
}
