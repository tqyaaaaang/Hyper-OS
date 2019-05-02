/**
 * dev/device_list.cpp
 * Devices list
 */

#include "device_list.h"

#include "devices/null/null.h"
#include "devices/clock/clock.h"

std::vector < device_t * > device_list ( {
	new dev_null ( 0 ),
	new dev_clock ( 0, std::chrono::milliseconds ( 100 ) )
} );
