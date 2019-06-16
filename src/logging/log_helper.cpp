/**
 * logging/log_helper.cpp
 * Helper functions for logging
 */

#include "log_helper.h"

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <thread>

#include "../env/env.h"
#include "../status/status.h"



std::string logging::get_time ()
{
	time_t cur_time = time ( NULL );

	struct tm * data = localtime ( &cur_time );

	std::stringstream string_helper;

	string_helper << std::setw ( 4 ) << std::setfill ( '0' ) << data->tm_year + 1900 << "-"
		<< std::setw ( 2 ) << std::setfill ( '0' ) << data->tm_mon + 1 << "-"
		<< std::setw ( 2 ) << std::setfill ( '0' ) << data->tm_mday << " "
		<< std::setw ( 2 ) << std::setfill ( '0' ) << data->tm_hour << ":"
		<< std::setw ( 2 ) << std::setfill ( '0' ) << data->tm_min << ":"
		<< std::setw ( 2 ) << std::setfill ( '0' ) << data->tm_sec << std::endl;
	std::string result;
	getline ( string_helper, result );

	return result;
}

std::string logging::get_level ( int level )
{
	std::string result;
	switch ( level ) {
	case LOGGING_LEVEL::CRITICAL:
		result = "CRITICAL";
		break;
	case LOGGING_LEVEL::ERROR:
		result = "ERROR";
		break;
	case LOGGING_LEVEL::WARNING:
		result = "WARNING";
		break;
	case LOGGING_LEVEL::INFO:
		result = "INFO";
		break;
	case LOGGING_LEVEL::DEBUG:
		result = "DEBUG";
		break;
	default:
		std::stringstream string_helper;
		string_helper << level;
		string_helper >> result;
		break;
	}
	std::stringstream string_helper;
	string_helper << std::setw ( 7 ) << std::left << result;
	getline ( string_helper, result );
	return result;
}

std::string logging::get_thread ()
{
	std::stringstream string_helper;
	string_helper << std::this_thread::get_id () << " (";
	if ( status.get_name () != "" ) {
		string_helper << " " << status.get_name () << " ";
	}
	string_helper << ")";
	std::string result;
	getline ( string_helper, result );
	return result;
}

std::string logging::get_prefix ( int level )
{
	return get_time () + " : " + get_level ( level ) + " : " + get_thread () + " : ";
}
