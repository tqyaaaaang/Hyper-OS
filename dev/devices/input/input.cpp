/**
 * dev/devices/input/input.h
 * Device input
 */

#include "input.h"
#include "../../../src/logging/logging.h"

dev_input::dev_input ( int __motherboard_id )
	: device_t ( __motherboard_id )
{
}

void dev_input::init ()
{
	logging::debug << "Initializing device input" << logging::log_endl;
}

void dev_input::destroy ()
{
	logging::debug << "Destroying device input" << logging::log_endl;
}

std::string dev_input::to_string () const
{
	return "input";
}

char dev_input::read ()
{
	logging::debug << "Reading character from device input" << logging::log_endl;
	char ch;
	ch = getchar ();
	if ( ch >= 32 ) {
		logging::debug << "Device input received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device input received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	return ch;
}
