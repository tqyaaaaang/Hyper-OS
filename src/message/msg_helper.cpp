/**
 * message/msg_helper.h
 * Helper functions for message
 */

#include "msg_helper.h"
#include "message.h"
#include "../status/status.h"
#include "../core/core.h"
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

std::string message::wrap_core_info ( std::string str )
{
	std::stringstream string_helper;
	string_helper << str << " (" << status.get_core ()->get_core_id () << ")";
	return string_helper.str ();
}
