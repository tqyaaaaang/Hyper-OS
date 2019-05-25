/**
 * dev/devices/input/input_screen.h
 * Device input
 * Input from module screen
 */

#pragma once

#include "input.h"
#include "../../../src/status/status.h"
#include <string>

class dev_input_screen : public dev_input
{
public:
	dev_input_screen ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual int read ();

private:
	virtual void device_thread_event_loop ();
};
