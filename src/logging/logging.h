/**
 * logging/logging.h
 * Logging module
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

#include "log_helper.h"

namespace logging
{
	extern thread_local std::string BUF;
	extern std::ostream *OUT;
	extern std::mutex output_lock;

	void add_string ( std::string str );
	std::string get_string ();
	void clear_string ();

	/**
	* class log_endl_t
	* identifier to end a logger line
	*/
	class log_endl_t
	{

	};

	/**
	* class logger
	* can be used to write logs
	*/
	class logger
	{
	public:
		logger ();
		logger ( int _level );
		~logger ();

		template < typename T >
		friend logger & operator << ( logger &buf, T a )
		{
			if ( buf.visible ) {
				std::stringstream SS_BUF;
				SS_BUF << a;
				add_string ( SS_BUF.str () );
			}
			return buf;
		}

		friend logger & operator << ( logger &buf, log_endl_t a )
		{
			if ( buf.visible ) {
				{
					std::lock_guard < std::mutex > lock ( output_lock );

					( *OUT ) << get_prefix ( buf.level ) << get_string () << std::endl;
				}
				clear_string ();
			}
			return buf;
		}

	private:
		int level;
		bool visible;
	};

	extern logger critical;
	extern logger error;
	extern logger warning;
	extern logger info;
	extern logger debug;

	extern log_endl_t log_endl;
}

void init_logger ();   // Initialize the logger
void destroy_logger ();   // Destory the logger
