/**
 * dev/devices/screen/screen
 * Device screen
 * Low-level interact methods with screen
 */

#pragma once

#include "../device_t.h"
#include "../../../src/status/status.h"
#include <string>
#include <mutex>

class dev_screen : public device_t
{
public:
	dev_screen ( int __motherboard_id );

	virtual std::string to_string () const;

	virtual int read ();
	virtual void write ( std::string type, std::string data );

private:
	std::mutex write_lock;
};
