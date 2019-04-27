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

void CPU_core::enable ()
{
	enabled_flag = true;
}

void CPU_core::disable ()
{
	enabled_flag = false;
}

bool CPU_core::is_enabled () const
{
	return enabled_flag;
}

void CPU_core::enable_interrupt ()
{
	interrupt_enabled_flag = true;
}

void CPU_core::disable_interrupt ()
{
	interrupt_enabled_flag = false;
}

bool CPU_core::is_interrupt_enabled () const
{
	return interrupt_enabled_flag;
}
