/**
 * dev/devices/input/input.h
 * Device input
 */

#pragma once

#include "../device_t.h"
#include "../../../src/status/status.h"
#include <string>

class dev_input : public device_t
{
public:
	dev_input ( int __motherboard_id );

	virtual std::string to_string () const;

	virtual int read () = 0;

private:
};
