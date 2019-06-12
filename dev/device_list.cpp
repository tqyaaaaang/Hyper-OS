/**
 * dev/device_list.cpp
 * Devices list
 */

#include "device_list.h"

#include "devices/null/null.h"
#include "devices/clock/clock.h"
#include "utils/create_io_device_helper.h"

std::vector < device_t * > device_list ( {
	new dev_null ( 0 ),
	new dev_clock ( 0, std::chrono::milliseconds ( 20 ) ),
	make_input_device ( 0 ),
	make_output_device ( 0 ),
	new dev_screen ( 0 )
} );



namespace device_desc
{
	dev_input *standard_input = dynamic_cast < dev_input * > ( device_list[2] );
	dev_output *standard_output = dynamic_cast < dev_output * > ( device_list[3] );

	dev_screen *device_screen = dynamic_cast < dev_screen * > ( device_list[4] );
}
