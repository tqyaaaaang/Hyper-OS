/**
 * dev/devices/input/input_screen.cpp
 * Device input
 * Input from module screen
 */

#include "input_screen.h"
#include "../../../src/logging/logging.h"
#include "../../../src/message/message.h"

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

	device_thread.detach ();   // Cannot join because we cannot notify the thread to exit
}

int dev_input_screen::read ()
{
	return 0;
}

void dev_input_screen::device_thread_event_loop ()
{
	std::string line;
	while ( std::getline ( std::cin, line ) ) {
		logging::info << "device input received line : \"" << line << "\"" << logging::log_endl;
		message::test ( "dev input" ) << "device input received line : \"" << line << "\"" << message::msg_endl;
	}
}
