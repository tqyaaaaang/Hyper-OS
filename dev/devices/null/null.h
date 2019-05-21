/**
 * dev/devices/null/null.h
 * Device NULL
 */

#pragma once

#include "../device_t.h"

class dev_null : public device_t
{
public:
	dev_null ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual std::string to_string () const;

private:
};
