/**
 * dev/devices/screen/screen
 * Device screen
 * Low-level interact methods with screen
 */

#include "screen.h"
#include "../../../src/env/env.h"
#include "../../../src/status/status.h"
#include "../../../src/logging/logging.h"
#include "../../device_list.h"
#include "../input/input_screen.h"
#include <string>

dev_screen::dev_screen ( int __motherboard_id )
	: device_t ( __motherboard_id )
{
}

void dev_screen::init ()
{
	logging::debug << "Initializing device screen" << logging::log_endl;

	device_thread = std::thread ( &dev_screen::device_thread_entry, this, status );
}

void dev_screen::destroy ()
{
	logging::debug << "Destroying device screen" << logging::log_endl;

	if ( IO_DEVICE ) {
		write ( "s", "shutdown" );
	}
	device_thread.join ();
}

std::string dev_screen::to_string () const
{
	return "screen";
}

int dev_screen::read ()
{
	return 0;
}

void dev_screen::write ( std::string type, std::string data )
{
	std::unique_lock < std::mutex > lck ( write_lock );

	std::cout << type << "[" << data << "]" << std::endl;
}

void dev_screen::write ( std::string type, std::string config, std::string data )
{
	std::unique_lock < std::mutex > lck ( write_lock );

	std::cout << type << "{" << config << "}" << "[" << data << "]" << std::endl;
}

void dev_screen::write_char ( char ch )
{
	std::string str;
	str = ch;
	write ( "w", str );
}

void dev_screen::backspace ()
{
	write ( "c", "backspace" );
}

void dev_screen::newline ()
{
	write ( "c", "newline" );
}

void dev_screen::cursor_move_left ()
{
	write ( "c", "cursor_move_left" );
}

void dev_screen::cursor_move_right ()
{
	write ( "c", "cursor_move_right" );
}

void dev_screen::device_thread_event_loop ()
{
	if ( IO_DEVICE ) {
		logging::debug << "device screen daemon thread up" << logging::log_endl;
		std::string line;
		while ( std::getline ( std::cin, line ) ) {
			logging::debug << "device screen received line : \"" << line << "\"" << logging::log_endl;

			std::string data = line.substr ( 1 );
			if ( data[0] == '[' && data[data.size () - 1] == ']' ) {
				data = data.substr ( 1, data.size () - 2 );
			}

			switch ( line[0] ) {
			case 'k':
				dynamic_cast < dev_input_screen * > ( device_desc::standard_input )->send_key ( data );
				break;

			case 's':
				if ( data == "shutdown" ) {
					return;
				}
				logging::warning << "device screen received unexpected system command : " << data << logging::log_endl;
				break;

			default:
				logging::error << "device screen receive type " << line[0] << " which is not implemented" << logging::log_endl;
			}
		}
	}
}
