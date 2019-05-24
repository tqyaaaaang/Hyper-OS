/**
 * dev/devices/output/output_screen.h
 * Device output
 * Output to module teminal
 */

#include "output_screen.h"
#include "../../../src/logging/logging.h"

dev_output_screen::dev_output_screen ( int __motherboard_id )
	: dev_output ( __motherboard_id )
{
}

void dev_output_screen::init ()
{
	logging::debug << "Initializing device output, mode: screen" << logging::log_endl;
}

void dev_output_screen::destroy ()
{
	logging::debug << "Destroying device output, mode: screen" << logging::log_endl;
}

void dev_output_screen::write ( char ch )
{
	if ( ch >= 32 ) {
		logging::debug << "Device output received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	} else {
		logging::debug << "Device output received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
	}
	if ( ch == '\n' ) {
		printf ( "w[\\n]\n" );
	} else {
		printf ( "w[%c]\n", ch );
	}
	fflush ( stdout );
}
