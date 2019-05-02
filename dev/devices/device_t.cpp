/**
 * dev/device_t.cpp
 * Devices base class
 */

#include "device_t.h"
#include "../../src/motherboard/motherboard.h"
#include "../../src/logging/logging.h"
#include <sstream>

device_t::device_t ()
{
}

device_t::device_t ( int __motherboard_id )
	: _motherboard_id ( __motherboard_id )
{
}

device_t::device_t ( motherboard_t * _motherboard )
	: motherboard ( _motherboard )
{
}

void device_t::set_id ( int _id )
{
	id = _id;
}

int device_t::get_id () const
{
	return id;
}

void device_t::init_motherboard ()
{
	motherboard = &motherboards[_motherboard_id];
}

void device_t::set_motherboard ( motherboard_t * _motherboard )
{
	motherboard = _motherboard;
}

motherboard_t * device_t::get_motherboard () const
{
	return motherboard;
}

std::string device_t::to_string () const
{
	return "";
}

void device_t::device_thread_entry ( status_t father_thread_status )
{
	status = father_thread_status;
	status.set_core ( NULL );
	std::stringstream string_helper;
	string_helper << "device #" << get_id () << " (" << to_string () << ")";
	status.set_name ( string_helper.str () );
	logging::debug << "Device #" << get_id () << " (" << to_string () << ") created" << logging::log_endl;

	device_thread_event_loop ();
}

void device_t::device_thread_event_loop ()
{
	logging::error << "Device thread event loop of device " << to_string () << " not implemented" << logging::log_endl;
}
