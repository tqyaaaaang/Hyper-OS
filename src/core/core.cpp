/**
 * core/core.h
 * CPU Core
 */

#include "core.h"
#include "../logging/logging.h"

CPU_core::CPU_core ()
	: enabled_flag ( false )
	, interrupt_enabled_flag ( false )
	, lapic ( this )
{
}

CPU_core::~CPU_core ()
{
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
