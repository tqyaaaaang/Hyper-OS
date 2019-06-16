/**
 * dev/devices/input/input_screen.h
 * Device input
 * Input from module screen
 */

#pragma once

#include "input.h"
#include "../../../src/status/status.h"
#include "../../../src/utils/thread_safe_queue/thread_safe_queue.h"
#include <string>

class dev_input_screen : public dev_input
{
public:
	dev_input_screen ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual int read ();

	virtual void send_key ( std::string key );

private:
	virtual void device_thread_event_loop ();

	thread_safe_queue < std::string > event_queue;
};
