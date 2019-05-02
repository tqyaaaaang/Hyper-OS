/**
 * status/status.cpp
 * Context of the current thread
 */

#include "status.h"
#include "../core/cpus.h"
#include "../logging/logging.h"
#include "../core/core.h"

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


void status_t::set_page_table(page_table *pgt)
{
	this->pgt = pgt;	
}

page_table * status_t::get_page_table() const
{
	return this->pgt;
}

thread_local status_t status;

void init_status_phase_1 ()
{
	logging::debug << "Setting initial thread status - Phase 1" << logging::log_endl;
	status.set_name ( "main thread" );
}

void init_status_phase_2 ()
{
	logging::debug << "Setting initial thread status - Phase 2" << logging::log_endl;
	status.set_core ( &cores[0] );
}
