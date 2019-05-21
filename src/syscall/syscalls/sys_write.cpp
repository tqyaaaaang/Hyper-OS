/**
 * syscall/syscalls/sys_write.cpp
 * write to device syscall
 */

#include "sys_write.h"

sys_write::sys_write(dev_output *device, char data)
	: syscall_t(syscall_id_t::WRITE)
{
	this->device = device;
	this->data = data;
}

int sys_write::process()
{
	device->write(data);
	return 0;
}
