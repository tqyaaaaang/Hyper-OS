/**
 * dev/devices/input/input_screen.cpp
 * Device input
 * Input from module screen
 */

#include "input_screen.h"
#include "../../../src/logging/logging.h"
#include "../../../src/message/message.h"
#include "../../../src/process/process.h"
#include "../../../src/schedule/signal.h"
#include "../../../src/status/status.h"
#include "../../../src/core/core.h"
#include "../../../src/motherboard/motherboard.h"
#include "../../../src/interrupt/interrupts/keyboard_interrupt.h"

dev_input_screen::dev_input_screen ( int __motherboard_id )
	: dev_input ( __motherboard_id )
{
}

void dev_input_screen::init ()
{
	logging::debug << "Initializing device input, mode: screen" << logging::log_endl;

	device_thread = std::thread ( &dev_input_screen::device_thread_entry, this, status );
}

void dev_input_screen::destroy ()
{
	logging::debug << "Destroying device input, mode: screen" << logging::log_endl;

	event_queue.push_front ( "<exit>" );   // Send exit signal
	device_thread.join ();
}

int dev_input_screen::read ()
{
	if ( status.get_core ()->get_current () == nullptr ) {
		logging::error << "Read from kernel is not implemented" << logging::log_endl;
		return 0;
	}
	proc_wait ( signal_id::KEYBOARD );
	return 0;
}

void dev_input_screen::send_key ( std::string key )
{
	event_queue.push_back ( key );
}

void dev_input_screen::device_thread_event_loop ()
{
	while ( 1 ) {
		std::string data = event_queue.pop_front ();

		if ( data == "<exit>" ) {   // Handle exit signal
			break;
		}

		logging::info << "device input received line : \"" << data << "\"" << logging::log_endl;
		message::test ( "dev input" ) << "device input received line : \"" << data << "\"" << message::msg_endl;

		get_motherboard ()->send_interrupt ( new keyboard_interrupt ( data ) );
	}
}
