/**
 * dev/device_list.cpp
 * Devices list
 */

#include "device_list.h"

#include "devices/null/null.h"
#include "devices/clock/clock.h"
#include "devices/output/output.h"

std::vector < device_t * > device_list ( {
	new dev_null ( 0 ),
	new dev_clock ( 0, std::chrono::milliseconds ( 300 ) ),
	new dev_output ( 0 )
} );



namespace device_desc
{
	dev_output *standard_output = dynamic_cast < dev_output * > ( device_list[2] );
}
