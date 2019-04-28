/**
 * status/status.cpp
 * Context of the current thread
 */

#include "status.h"
#include "../core/cpus.h"
#include "../logging/logging.h"

status_t::status_t ()
	: current_core ( NULL )
	, name ( "" )
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

void status_t::set_name ( std::string _name )
{
	name = _name;
}

std::string status_t::get_name () const
{
	return name;
}

thread_local status_t status;

void init_status ()
{
	logging::info << "Setting initial thread status" << logging::log_endl;
	status.set_core ( &cores[0] );
	status.set_name ( "main thread" );
}
