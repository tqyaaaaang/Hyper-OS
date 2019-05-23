/**
 * program/sys_t.cpp
 * syscall interface for user-mode programs
 */

#include "sys_t.h"
#include "program_manager.h"
#include "../../dev/device_list.h"
#include "../../dev/devices/output/output.h"
#include "../../dev/devices/input/input.h"
#include "../process/process.h"
#include "../process/process_t.h"
#include "../interrupt/interrupt.h"
#include "../syscall/syscall.h"
#include "../syscall/syscalls/sys_create_proc.h"
#include "../syscall/syscalls/sys_exec_prog.h"
#include "../syscall/syscalls/sys_write.h"
#include "../syscall/syscalls/sys_read.h"
#include "../syscall/syscalls/sys_exit.h"
#include "../syscall/syscalls/sys_wait.h"
#include "../syscall/syscalls/sys_yield.h"
#include "../interrupt/interrupts/syscall_interrupt.h"
#include "../logging/logging.h"
#include "program.h"

using std::string;

sys_t::sys_t(program *prog)
{
	this->prog = prog;
}

int sys_t::intr(syscall_t *sys)
{
	int result = syscall(sys);
	int return_value = sys->get_return_value();
	delete sys;
	check_interrupt ();
	return return_value;
}

int sys_t::create_process()
{
	return intr(new sys_create_proc());
}

int sys_t::exec_program(int pid, handle<char> name)
{
	std::string str;
	for (size_t i = 0; name[i] != '\0'; i++)
		str = str + (char)name[i];
	return intr(new sys_exec_prog(pid, get_program(str)));
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
	int w = intr(new sys_wait(pid));
	return w;
}

int sys_t::read(dev_input *device)
{
	int w = intr(new sys_read(device));
	return w;
}

int sys_t::write(dev_output *device, char data)
{
	return intr(new sys_write(device, data));
}

dev_output* sys_t::std_output()
{
	return device_desc::standard_output;
}

dev_input* sys_t::std_input()
{
	return device_desc::standard_input;
}
