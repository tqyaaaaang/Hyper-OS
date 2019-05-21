/**
 * dev/devices/output/output.h
 * Device output
 */

#include "output.h"
#include "../../../src/logging/logging.h"

dev_output::dev_output ( int __motherboard_id )
	: device_t ( __motherboard_id )
{
}

void dev_output::init ()
{
	logging::debug << "Initializing device output" << logging::log_endl;
}

void dev_output::destroy ()
{
	logging::debug << "Destroying device output" << logging::log_endl;
}

std::string dev_output::to_string () const
{
	return "output";
}

void dev_output::write ( char ch )
{
	if ( ch >= 32 ) {
		logging::debug << "Device output received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device output received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	putchar ( ch );
}
