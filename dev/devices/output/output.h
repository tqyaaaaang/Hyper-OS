/**
 * dev/devices/output/output.h
 * Device output
 */

#pragma once

#include "../device_t.h"
#include "../../../src/status/status.h"
#include <string>

class dev_output : public device_t
{
public:
	dev_output ( int __motherboard_id );

	virtual std::string to_string () const;

	virtual void write ( char ch ) = 0;
	virtual void write ( std::string str );

private:
};
