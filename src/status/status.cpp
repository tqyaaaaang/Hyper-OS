/**
 * status/status.cpp
 * Context of the current thread
 */

#include "status.h"
#include "../core/cpus.h"
#include "../logging/logging.h"

status_t::status_t ()
	: current_core ( NULL )
{
}

void status_t::set_core ( CPU_core * core )
{
	current_core = core;
}

CPU_core * status_t::get_core () const
{
	return current_core;
}

thread_local status_t context;

void init_status ()
{
	logging::info << "Set initial thread status" << logging::log_endl;
	context.set_core ( &cores[0] );
}
