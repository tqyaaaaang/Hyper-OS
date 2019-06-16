#include "demo_interrupt.h"
#include "../../src/program/program_manager.h"
#include "../../src/message/message.h"
#include "../../src/interrupt/interrupt.h"
#include "../../src/interrupt/interrupts/double_internal_test.h"
#include <cstring>

static program* gen()
{
	return new demo_interrupt;
}

void register_demo_syscall()
{
	register_program("demo-interrupt", gen);
}

demo_interrupt::demo_interrupt()
{ }

demo_interrupt::~demo_interrupt()
{ }

void demo_interrupt::static_init()
{
	set_name("demo-interrupt");
	hello = alloc_static<char>(2);
	hello.modify_in_compile('~');
	hello[1].modify_in_compile('\0');
}

void demo_interrupt::main()
{
	message::set_interrupt_message(true);
	message::set_process_message(false);
	message::set_memory_message(false);

	message::interrupt ( "user demo_interrupt" ) << "Sending interrupt DOUBLE_INTERRUPT_TEST" << message::msg_endl;
	int return_value = interrupt ( new double_internal_test_interrupt () );
	message::interrupt ( "user demo_interrupt" ) << "Interrupt DOUBLE_INTERRUPT_TEST return value : " << return_value << message::msg_endl;

	message::interrupt ( "user demo_interrupt" ) << "Sending syscalls to print \"~\\n\"" << message::msg_endl;
	hos_std->println(hello);
	message::interrupt ( "user demo_interrupt" ) << "Print completed" << message::msg_endl;

	message::interrupt ( "user demo_interrupt" ) << "Waiting for clock interrupt" << message::msg_endl;
	handle < int > timer = alloc_stack < int > ();
	handle < int > answer = alloc_stack < int > ();
	answer = 0;
	for ( timer = 0; timer < 10; timer = timer + 1 ) {
		answer = answer + ( ( timer >> 3 ) ^ ( timer << 7 ) );
	}
	message::interrupt ( "user demo_interrupt" ) << "Waiting completed" << message::msg_endl;

	message::set_interrupt_message(false);
	message::set_process_message(false);
	message::set_memory_message(false);
}
