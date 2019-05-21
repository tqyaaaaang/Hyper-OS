/**
 * syscall/syscalls/sys_wait.cpp
 * create_proc syscall
 */

#include "sys_yield.h"
#include "../../process/process.h"

sys_wait::sys_wait(int pid)
	:syscall_t(syscall_id_t::WAIT)
{
	this->pid = pid;
}

int sys_wait::process()
{
	return proc_wait(pid);
}
