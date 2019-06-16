/**
 * dev/devices/output/output_terminal.h
 * Device output
 * Output directly to teminal
 */

#include "output_terminal.h"
#include "../../../src/logging/logging.h"

dev_output_terminal::dev_output_terminal ( int __motherboard_id )
	: dev_output ( __motherboard_id )
{
}

void dev_output_terminal::init ()
{
	logging::debug << "Initializing device output, mode: terminal" << logging::log_endl;
}

void dev_output_terminal::destroy ()
{
	logging::debug << "Destroying device output, mode: terminal" << logging::log_endl;
}

void dev_output_terminal::write ( int ch )
{
	if ( ch >= 32 ) {
		logging::debug << "Device output received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device output received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	putchar ( ch );
	fflush ( stdout );
}
