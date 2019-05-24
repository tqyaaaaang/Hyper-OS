/**
 * dev/devices/clock/clock.cpp
 * Device clock
 */

#include "clock.h"
#include "../../../src/motherboard/motherboard.h"
#include "../../../src/interrupt/interrupts/clock_interrupt.h"
#include <sstream>
#include "../../../src/logging/logging.h"

dev_clock::dev_clock ( int __motherboard_id, std::chrono::nanoseconds _tick_duration )
	: device_t ( __motherboard_id )
	, tick_duration ( _tick_duration )
{
}

void dev_clock::init ()
{
	device_t::init ();

	device_thread = std::thread ( &dev_clock::device_thread_entry, this, status );
}

void dev_clock::destroy ()
{
	device_t::destroy ();
	
	prom.set_value ( 0 );
	device_thread.join ();
}

std::string dev_clock::to_string () const
{
	return "clock";
}

void dev_clock::device_thread_event_loop ()
{
	logging::debug << "Device clock entered event loop" << logging::log_endl;
	auto wait_future = prom.get_future ();
	while ( true ) {
		if ( wait_future.wait_for ( tick_duration ) == std::future_status::timeout ) {
			logging::debug << "Device clock sending a tick" << logging::log_endl;
			get_motherboard ()->send_interrupt ( new clock_interrupt () );
		} else {
			logging::debug << "Device clock received kill signal" << logging::log_endl;
			return;
		}
	}
}
