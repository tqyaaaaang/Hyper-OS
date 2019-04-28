/**
 * apic/local_apic.cpp
 * Local APIC
 */


#include "local_apic.h"

#include <sstream>

#include "../core/core.h"
#include "../logging/logging.h"
#include "../status/status.h"

local_apic::local_apic ( CPU_core *_core )
	: core ( _core )
{
}

local_apic::~local_apic ()
{
	if ( enabled ) {
		lapic_thread.join ();
	}
}

void local_apic::enable ()
{
	logging::debug << "Enabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
	enabled = true;
	lapic_thread = std::thread ( &local_apic::lapic_thread_entry, this, status );
}

void local_apic::disable ()
{
	logging::debug << "Enabling local APIC for CPU #" << core->get_core_id () << logging::log_endl;
	enabled = false;
	lapic_thread.join ();
}

bool local_apic::is_enabled () const
{
	return enabled;
}

void local_apic::lapic_thread_entry ( status_t father_thread_status )
{
	status = father_thread_status;
	std::stringstream string_helper;
	string_helper << "LAPIC of CPU #" << core->get_core_id ();
	status.set_name ( string_helper.str () );
	logging::debug << "Local APIC for CPU #" << core->get_core_id () << " created" << logging::log_endl;
}
