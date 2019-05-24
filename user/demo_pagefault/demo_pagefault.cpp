#include "demo_pagefault.h"
#include "../../src/program/program_manager.h"
#include "../../src/message/message.h"

static program* gen()
{
	return new demo_pagefault;
}

void register_demo_pagefault()
{
	register_program("demo-pf", gen);
}

demo_pagefault::demo_pagefault()
{ }

demo_pagefault::~demo_pagefault()
{ }

void demo_pagefault::static_init()
{
	set_name("demo_pagefault");
}

void demo_pagefault::main()
{
	message::set_interrupt_message(false);
	message::set_process_message(false);
	message::set_memory_message(true);
	handle<char> variable_on_stack = '1';
	message::set_interrupt_message(false);
	message::set_process_message(false);
	message::set_memory_message(false);
}
