/**
 * syscall/syscalls/sys_create_proc.cpp
 * create_proc syscall
 */
#include "sys_create_proc.h"

sys_create_proc::sys_create_proc()
{}

int sys_create_proc::process()
{
	return create_process();
}
