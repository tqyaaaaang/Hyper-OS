/**
 * dev/utils/create_io_device_helper.cpp
 * Helper functions to create I/O Devices
 */

#include "create_io_device_helper.h"
#include "../devices/output/output_terminal.h"
#include "../devices/output/output_screen.h"
#include "../../src/env/env.h"

device_t * make_input_device ( int motherboard_id )
{
	return nullptr;
}

device_t * make_output_device ( int motherboard_id )
{
	if ( IO_DEVICE ) {
		return new dev_output_screen ( motherboard_id );
	} else {
		return new dev_output_terminal ( motherboard_id );
	}
}
