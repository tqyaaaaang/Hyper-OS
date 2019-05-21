/**
 * syscall/syscalls/sys_create_proc.cpp
 * create_proc syscall
 */
#include "sys_create_proc.h"

sys_create_proc::sys_create_proc()
	:syscall_t(syscall_id_t::CREATE_PROC)
{}

int sys_create_proc::process()
{
	return proc_create_process();
}
