/**
 * syscall/syscalls/sys_yield.cpp
 * create_proc syscall
 */

#include "sys_yield.h"
#include "../../process/process.h"

sys_yield::sys_yield()
	:syscall_t(syscall_id_t::YIELD)
{}

int sys_yield::process()
{
	return proc_yield();
}
