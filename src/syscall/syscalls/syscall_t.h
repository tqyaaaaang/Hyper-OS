/**
 * syscall/syscalls/syscall_t.h
 * System call classes
 * Abstract base class syscall_t
 */

#pragma once

#include "../syscall_id.h"
#include <string>

class syscall_t
{
public:
	syscall_t ( syscall_id_t id );
	virtual ~syscall_t();
	
	void set_syscalL_id ( syscall_id_t id );
	syscall_id_t get_syscall_id () const;

	void set_return_value ( int value );
	int get_return_value () const;

	virtual int process () = 0;

	virtual std::string to_string ();

protected:
	syscall_id_t syscall_id;
	int return_value;
};
