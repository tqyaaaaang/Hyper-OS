/**
 * dev/utils/create_io_device_helper.h
 * Helper functions to create I/O Devices
 */

#pragma once

class device_t;

device_t * make_input_device ( int motherboard_id );
device_t * make_output_device ( int motherboard_id );
