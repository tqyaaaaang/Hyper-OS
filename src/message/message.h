/**
 * message/message.h
 * Message module
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>
#include <functional>
#include <memory>

#include "msg_helper.h"

namespace message
{
	extern std::ostream *OUT;
	extern std::mutex output_lock;

	/**
	 * class msg_info_t
	 * information of a message
	 */
	class msg_info_t
	{
	public:
		msg_info_t ();
		msg_info_t ( std::string type, bool visible );

		void set_type ( std::string _type );
		std::string get_type () const;

		void set_visible ( bool _visible );
		bool get_visible () const;

		std::string type;
		bool visible;
	};

	/**
	* class msg_endl_t
	* identifier to end a message line
	*/
	class msg_endl_t
	{

	};

	/**
	* class message
	* can be used to write messages
	*/
	class message
	{
	public:
		message ( msg_info_t _info, std::string _source );
		message ( const message &_message );
		~message ();

		template < typename T >
		friend message & operator << ( message &buf, T a )
		{
			logging::debug << "message output : " << a << logging::log_endl;
			if ( buf.info.visible ) {
				buf.BUF << a;
			}
			return buf;
		}

		friend message & operator << ( message &buf, msg_endl_t a )
		{
			if ( buf.info.visible ) {
				{
					std::lock_guard < std::mutex > lock ( output_lock );

					( *OUT ) << get_prefix ( buf.info, buf.source ) << buf.BUF.str () << get_suffix ( buf.info, buf.source ) << std::endl;
				}
				buf.BUF.clear ();
				buf.BUF.str ( "" );
			}
			return buf;
		}

	private:
		msg_info_t info;
		std::string source;
		std::stringstream BUF;
	};

	/**
	 * class message_wrapper
	 * wrapper of message instance
	 */
	class message_wrapper
	{
	public:
		message_wrapper ( message msg );
		~message_wrapper ();

		template < typename T >
		friend message_wrapper operator << ( message_wrapper buf, T a )
		{
			( *buf.ptr ) << a;
			return buf;
		}

	private:
		std::shared_ptr < message > ptr;
	};

	message make_message ( msg_info_t _info, std::string _source );
	message_wrapper wrap_message ( message msg );
	message_wrapper make_wrap_message ( msg_info_t _info, std::string _source );

	extern msg_endl_t msg_endl;

	typedef decltype ( std::bind ( make_wrap_message, msg_info_t (), std::placeholders::_1 ) ) alias_type;
	extern const alias_type msg_alias;

	alias_type make_alias ( msg_info_t _info );

	extern alias_type interrupt;
	extern alias_type test;
}

void init_message ();   // Initialize the message objects
void destroy_message ();   // Destory the message objects
