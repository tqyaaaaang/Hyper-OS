/**
 * syscall/syscalls/sys_read.h
 * read from device syscall
 */

#include "sys_read.h"
#include "../../logging/logging.h"

sys_read::sys_read ( dev_input * device )
	: syscall_t ( syscall_id_t::READ )
	, device ( device )
{
}

int sys_read::process ()
{
	logging::debug << "Processing syscall READ" << logging::log_endl;
	char ch;
	ch = device->read ();
	logging::debug << "Received character " << static_cast < int > ( ch ) << logging::log_endl;
	return ch;
}
