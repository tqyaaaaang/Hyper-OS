/**
 * syscall/syscalls/sys_read.h
 * read from device syscall
 */

#include "sys_read.h"

sys_read::sys_read ( dev_input * device, char data )
	: syscall_t ( syscall_id_t::READ )
	, device ( device )
	, data ( data )
{
}

int sys_read::process ()
{
	return device->read ();
}
