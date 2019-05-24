#include "demo_syscall.h"
#include "../../src/program/program_manager.h"
#include "../../src/message/message.h"
#include <cstring>

static program* gen()
{
	return new demo_syscall;
}

void register_demo_syscall()
{
	register_program("demo-syscall", gen);
}

demo_syscall::demo_syscall()
{ }

demo_syscall::~demo_syscall()
{ }

void demo_syscall::static_init()
{
	set_name("demo-syscall");
	hello = alloc_static<char>(2);
	hello.modify_in_compile('~');
	hello[1].modify_in_compile('\0');
}

void demo_syscall::main()
{
	message::set_interrupt_message(true);
	message::set_process_message(false);
	message::set_memory_message(false);
	hos_std->println(hello);
	message::set_interrupt_message(false);
	message::set_process_message(false);
	message::set_memory_message(false);
}
