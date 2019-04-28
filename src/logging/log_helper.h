/**
 * logging/log_helper.h
 * Helper functions for logging
 */

#include <string>

namespace logging
{
	std::string get_time ();
	std::string get_level ( int level );
	std::string get_thread ();

	std::string get_prefix ( int level );
}
