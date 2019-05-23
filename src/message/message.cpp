/**
 * message/message.cpp
 * Message module
 */

#include "message.h"
#include "../logging/logging.h"
#include <functional>

namespace message
{
	std::ostream *OUT;
	std::mutex output_lock;

	msg_endl_t msg_endl;

	const alias_type msg_alias = make_alias ( msg_info_t () );
	const alias_wrapper wrapped_msg_alias = wrap_alias ( msg_alias );
	alias_wrapper interrupt = wrapped_msg_alias;
	alias_wrapper test = wrapped_msg_alias;
}

message::msg_info_t::msg_info_t ()
	: type ( "" )
	, visible ( false )
{
}

message::msg_info_t::msg_info_t ( std::string type, bool visible )
	: type ( type )
	, visible ( visible )
{
}

void message::msg_info_t::set_type ( std::string _type )
{
	type = _type;
}

std::string message::msg_info_t::get_type () const
{
	return type;
}

void message::msg_info_t::set_visible ( bool _visible )
{
	visible = _visible;
}

bool message::msg_info_t::get_visible () const
{
	return visible;
}



message::message::message ( msg_info_t _info, std::string _source )
	: info ( _info )
	, source ( _source )
{
}

message::message::message ( const message & _message )
	: info ( _message.info )
	, source ( _message.source )
{
}

message::message::~message ()
{
	if ( !BUF.str ().empty () ) {
		logging::warning << "destructing a message instance which buffer is not empty : \"" << BUF.str () << "\"" << logging::log_endl;
	}
}

message::message_wrapper::message_wrapper ( message msg )
{
	ptr = std::make_shared < message > ( msg );
}

message::message_wrapper::~message_wrapper ()
{
}



message::message message::make_message ( msg_info_t _info, std::string _source )
{
	return message ( _info, _source );
}

message::message_wrapper message::wrap_message ( message msg )
{
	return message_wrapper ( msg );
}

message::message_wrapper message::make_wrap_message ( msg_info_t _info, std::string _source )
{
	return wrap_message ( make_message ( _info, _source ) );
}



message::alias_type message::make_alias ( msg_info_t _info )
{
	return std::bind ( make_wrap_message, static_cast < msg_info_t > ( _info ), std::placeholders::_1 );
}

message::alias_wrapper message::wrap_alias ( alias_type alias )
{
	return alias_wrapper ( alias );
}

message::alias_wrapper message::make_wrap_alias ( msg_info_t _info )
{
	return wrap_alias ( make_alias ( _info ) );
}



message::alias_wrapper::alias_wrapper ( alias_type value )
{
	ptr = std::make_shared < alias_type > ( value );
}

message::alias_wrapper::~alias_wrapper ()
{
}



void init_message ()
{
	logging::debug << "Initializing message service" << logging::log_endl;

	message::OUT = &std::cout;

	message::interrupt = message::make_wrap_alias ( message::msg_info_t ( "interrupt", true ) );
	message::test = message::make_wrap_alias ( message::msg_info_t ( "test", true ) );
}

void destroy_message ()
{
	logging::debug << "Message service destroyed" << logging::log_endl;
}
