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
#include "../interrupt/interrupts/syscall_interrupt.h"

using std::string;

static int intr(syscall_t *_sys)
{
	return interrupt(new syscall_interrupt(_sys));
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
	return 0;
}

int sys_t::exit()
{
	return 0;
}

int sys_t::wait(int pid)
{
	return 0;
}

int sys_t::read(int device)
{
	return 0;
}

int sys_t::write(int device, char data)
{
	return 0;
}

