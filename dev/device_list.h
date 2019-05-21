/**
 * dev/device_list.h
 * Devices list
 */

#pragma once

#include "devices/device_t.h"
#include "devices/output/output.h"
#include <vector>

extern std::vector < device_t * > device_list;

namespace device_desc
{
	extern dev_output *standard_output;
}
