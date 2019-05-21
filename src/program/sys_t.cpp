/**
 * program/sys_t.cpp
 * syscall interface for user-mode programs
 */

#include "sys_t.h"
#include "program_manager.h"
#include "../process/process.h"
#include "../interrupt/interrupt.h"
#include "../syscall/syscall.h"
#include "../syscall/syscalls/sys_create_proc.h"
#include "../syscall/syscalls/sys_exec_prog.h"
#include "../syscall/syscalls/sys_write.h"
#include "../syscall/syscalls/sys_exit.h"
#include "../syscall/syscalls/sys_wait.h"
#include "../syscall/syscalls/sys_yield.h"
#include "../interrupt/interrupts/syscall_interrupt.h"

using std::string;

static int intr(syscall_t *sys)
{
	if (interrupt(new syscall_interrupt(sys)) == 0) {
		int return_value = sys->get_return_value();
		delete sys;
		return return_value;
	}
	return -1;
}

int sys_t::create_process()
{
	return intr(new sys_create_proc());
}

int sys_t::exec_program(int pid, const string & name)
{
	return intr(new sys_exec_prog(pid, get_program(name)));
}

int sys_t::yield()
{
	return intr(new sys_yield());
}

int sys_t::exit()
{
	return intr(new sys_exit());
}

int sys_t::wait(int pid)
{
	return intr(new sys_wait(pid));
}

int sys_t::read(int device)
{
	return 0;
}

int sys_t::write(dev_output *device, char data)
{
	return intr(new sys_write(device, data));
}

dev_output* sys_t::std_output()
{
	return device_desc::standard_output;
}
