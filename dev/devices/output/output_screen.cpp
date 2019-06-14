/**
 * dev/devices/output/output_screen.h
 * Device output
 * Output to module teminal
 */

#include "output_screen.h"
#include "../../../src/logging/logging.h"
#include "../../device_list.h"
#include "../../../src/io/special_keys.h"

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

void dev_output_screen::write ( int ch )
{
	if ( ch >= 0 ) {
		if ( ch >= 32 ) {
			logging::debug << "Device output received character '" << ch << "' ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
		} else {
			logging::debug << "Device output received unvisible character ( ASCII " << static_cast < int > ( ch ) << " ) " << logging::log_endl;
		}

		if ( ch == '\b' ) {   // handle backspace
			device_desc::device_screen->backspace ();
		} else if ( ch == '\n' ) {
			device_desc::device_screen->newline ();
		} else {
			device_desc::device_screen->write_char ( static_cast < char > ( ch ) );
		}
	} else {
		special_keys key = static_cast < special_keys > ( -ch );
		logging::debug << "Device output received special key " << get_key_str ( key ) << logging::log_endl;

		switch ( key ) {
		case special_keys::LEFT:
			device_desc::device_screen->cursor_move_left ();
			break;

		case special_keys::RIGHT:
			device_desc::device_screen->cursor_move_right ();
			break;

		default:
			logging::warning << "Special key " << get_key_str ( key ) << " not handled" << logging::log_endl;
		}
	}
}
