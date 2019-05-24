/**
 * dev/devices/screen/screen
 * Device screen
 * Low-level interact methods with screen
 */

#include "screen.h"
#include "../../../src/logging/logging.h"

dev_screen::dev_screen ( int __motherboard_id )
	: device_t ( __motherboard_id )
{
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
	std::cout << type << "[" << data << "]" << std::endl;
}
