#include "demo_process.h"
#include "../../src/program/program_manager.h"
#include "../../src/message/message.h"
#include <cstring>

static program* gen()
{ return new demo_process; }

void register_demo_process()
{
	register_program("demo-proc", gen);
}

demo_process::demo_process()
{ }

demo_process::~demo_process()
{ }

void demo_process::static_init()
{
	set_name("demo-proc");
	const char str[] = "elephant";
	size_t size = strlen(str) + 1;
	name = alloc_static<char>(size);
	for (size_t i = 0; i < size; i++)
		name[i].modify_in_compile(str[i]);
}

void demo_process::main()
{	
	message::set_interrupt_message(false);
	message::set_process_message(true);
	message::set_memory_message(false);

	handle<int> pid = sys->create_process();
	sys->exec_program(pid, name);
	sys->wait(pid);

	hos_std->println(name);
	message::set_interrupt_message(false);
	message::set_process_message(false);
	message::set_memory_message(false);
}
