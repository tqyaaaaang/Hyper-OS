/**
 * dev/devices/output/output.h
 * Device output
 */

#pragma once

#include "../device_t.h"
#include "../../../src/status/status.h"

class dev_output : public device_t
{
public:
	dev_output ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual std::string to_string () const;

	virtual void write ( char ch );

private:
};
