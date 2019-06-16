/**
 * dev/devices/input/input_terminal.cpp
 * Device input
 * Input directly from teminal
 */

#include "input_terminal.h"
#include "../../../src/logging/logging.h"
#include "../../../src/status/status.h"
#include "../../../src/core/core.h"

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
	if ( status.get_core ()->get_current () == nullptr ) {
		logging::error << "Read from kernel is not implemented" << logging::log_endl;
		return 0;
	}
	logging::debug << "Reading character from device input" << logging::log_endl;
	char ch;
	ch = getchar ();
	status.get_core ()->get_current ()->set_signal_data ( ch );
	if ( ch >= 32 ) {
		logging::debug << "Device input received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device input received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	return 0;
}
