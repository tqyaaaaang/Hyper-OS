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

std::string dev_input::to_string () const
{
	return "input";
}
