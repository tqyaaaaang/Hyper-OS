/**
 * dev/devices/null/null.cpp
 * Device NULL
 */

#include "null.h"
#include "../../../src/logging/logging.h"

dev_null::dev_null ( int __motherboard_id )
	: device_t ( __motherboard_id )
{
}

std::string dev_null::to_string () const
{
	return "NULL";
}
