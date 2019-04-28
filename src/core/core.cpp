/**
 * core/core.h
 * CPU Core
 */

#include "core.h"
#include "../logging/logging.h"

CPU_core::CPU_core ()
	: enabled_flag ( false )
	, interrupt_enabled_flag ( false )
{
}

CPU_core::~CPU_core ()
{
}

void CPU_core::set_enabled ( bool status )
{
	enabled_flag = status;
}

void CPU_core::enable ()
{
	logging::info << "CPU #" << get_core_id () << " enabled" << logging::log_endl;
	set_enabled ( true );
}

void CPU_core::disable ()
{
	logging::info << "CPU #" << get_core_id () << " disabled" << logging::log_endl;
	set_enabled ( false );
}

bool CPU_core::is_enabled () const
{
	return enabled_flag;
}

void CPU_core::set_interrupt_enabled ( bool status )
{
	interrupt_enabled_flag = status;
}

void CPU_core::enable_interrupt ()
{
	logging::debug << "Enabling interrupt of CPU #" << get_core_id () << logging::log_endl;
	set_interrupt_enabled ( true );
}

void CPU_core::disable_interrupt ()
{
	logging::debug << "Disabling interrupt of CPU #" << get_core_id () << logging::log_endl;
	set_interrupt_enabled ( false );
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
