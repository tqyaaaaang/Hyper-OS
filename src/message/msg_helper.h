/**
 * message/msg_helper.h
 * Helper functions for message
 */

#include <string>

namespace message
{
	class msg_info_t;

	std::string get_prefix ( const msg_info_t &info, std::string source );
	std::string get_suffix ( const msg_info_t &info, std::string source );

	void output_message ( std::string msg );

	std::string wrap_core_info ( std::string str );
}
