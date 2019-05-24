/**
 * dev/devices/input/input_terminal.cpp
 * Device input
 * Input directly from teminal
 */

#include "input_terminal.h"
#include "../../../src/logging/logging.h"

dev_input_terminal::dev_input_terminal ( int __motherboard_id )
	: dev_input ( __motherboard_id )
{
}

void dev_input_terminal::init ()
{
	logging::debug << "Initializing device input, mode: terminal" << logging::log_endl;
}

void dev_input_terminal::destroy ()
{
	logging::debug << "Destroying device input, mode: terminal" << logging::log_endl;
}

int dev_input_terminal::read ()
{
	logging::debug << "Reading character from device input" << logging::log_endl;
	char ch;
	ch = getchar ();
	if ( ch >= 32 ) {
		logging::debug << "Device input received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device input received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	return ch;
}
