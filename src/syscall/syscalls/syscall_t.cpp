/**
 * syscall/syscalls/syscall_t.h
 * System call classes
 * Abstract base class syscall_t
 */

#include "syscall_t.h"

#include <sstream>

syscall_t::syscall_t ( syscall_id_t id )
	: syscall_id ( id )
{
}

syscall_t::~syscall_t ()
{
	
}

void syscall_t::set_syscalL_id ( syscall_id_t id )
{
	syscall_id = id;
}

syscall_id_t syscall_t::get_syscall_id () const
{
	return syscall_id;
}

void syscall_t::set_return_value ( int value )
{
	return_value = value;
}

int syscall_t::get_return_value () const
{
	return return_value;
}

std::string syscall_t::to_string ()
{
	std::stringstream string_buf;
	string_buf << "id : " << static_cast < int > ( get_syscall_id () );
	return string_buf.str ();
}
