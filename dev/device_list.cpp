/**
 * dev/device_list.cpp
 * Devices list
 */

#include "device_list.h"

#include "devices/null/null.h"
#include "devices/clock/clock.h"
#include "devices/output/output_terminal.h"
#include "devices/output/output_screen.h"

std::vector < device_t * > device_list ( {
	new dev_null ( 0 ),
	new dev_clock ( 0, std::chrono::milliseconds ( 200 ) ),
	new dev_input ( 0 ),
	new dev_output_screen ( 0 )
} );



namespace device_desc
{
	dev_input *standard_input = dynamic_cast < dev_input * > ( device_list[2] );
	dev_output *standard_output = dynamic_cast < dev_output * > ( device_list[3] );
}
