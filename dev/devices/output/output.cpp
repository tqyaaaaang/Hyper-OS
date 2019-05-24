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

std::string dev_output::to_string () const
{
	return "output";
}

void dev_output::write ( std::string str )
{
	for ( auto x : str ) {
		write ( x );
	}
}
