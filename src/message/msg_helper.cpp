/**
 * message/msg_helper.h
 * Helper functions for message
 */

#include "msg_helper.h"
#include "message.h"
#include "../status/status.h"
#include "../core/core.h"
#include "../../dev/device_list.h"
#include <sstream>

std::string message::get_prefix ( const msg_info_t & info, std::string source )
{
	std::stringstream string_helper;
	string_helper << source;
	while ( string_helper.str ().size () < 20 ) {
		string_helper << " ";
	}
	string_helper << " : ";
	return string_helper.str ();
}

std::string message::get_suffix ( const msg_info_t & info, std::string source )
{
	std::stringstream string_helper;
	return string_helper.str ();
}

void message::output_message ( std::string msg )
{
	if ( OUT != nullptr && ( IO_DEVICE == 1 || OUT != &std::cout ) ) {
		( *OUT ) << msg << std::endl;
	}
	if ( IO_DEVICE ) {
		device_desc::device_screen->write ( "m", msg );
	} else {
		std::cout << msg << std::endl;
	}
}

std::string message::wrap_core_info ( std::string str )
{
	std::stringstream string_helper;
	string_helper << str << " (" << status.get_core ()->get_core_id () << ")";
    return string_helper.str ();
}
