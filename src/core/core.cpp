/**
 * core/core.h
 * CPU Core
 */

#include "core.h"

CPU_core::CPU_core ()
{
	disable ();
	disable_interrupt ();
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
	set_enabled ( true );
}

void CPU_core::disable ()
{
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
	set_interrupt_enabled ( true );
}

void CPU_core::disable_interrupt ()
{
	set_interrupt_enabled ( false );
}

bool CPU_core::is_interrupt_enabled () const
{
	return interrupt_enabled_flag;
}
