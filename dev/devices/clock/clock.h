/**
 * dev/devices/clock/clock.h
 * Device clock
 */

#pragma once

#include "../device_t.h"
#include <chrono>
#include <future>
#include "../../../src/status/status.h"

class dev_clock : public device_t
{
public:
	dev_clock ( motherboard_t *_motherboard, std::chrono::nanoseconds _tick_duration = std::chrono::milliseconds ( 1 ) );

	virtual void init ();
	virtual void destroy ();

	virtual std::string to_string () const;

private:
	virtual void device_thread_event_loop ();

	std::chrono::nanoseconds tick_duration;

	std::promise < int > prom;
};
