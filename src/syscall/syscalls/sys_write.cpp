/**
 * syscall/syscalls/sys_write.cpp
 * write to device syscall
 */

#include "sys_write.h"
#include "../../logging/logging.h"

sys_write::sys_write ( dev_output *device, char data )
	: syscall_t ( syscall_id_t::WRITE )
{
	this->device = device;
	this->data = data;
}

int sys_write::process ()
{
	logging::debug << "Writing character " << static_cast < int > ( data ) << " to output device" << logging::log_endl;
	device->write ( data );
	return 0;
}
