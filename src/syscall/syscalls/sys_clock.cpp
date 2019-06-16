/**
 * syscall/syscalls/sys_clock.cpp
 * clock()
 */
#include "sys_clock.h"
#include "../../core/core.h"
#include "../../status/status.h"
#include "../../process/process_t.h"
#include "../syscall_id.h"

sys_clock::sys_clock()
	: syscall_t(syscall_id_t::CLOCK) 
{
	
}

int sys_clock::process()
{
	return status.get_core()->get_current()->get_clock_counter();
}
