/**
 * interrupt/interrupts/syscall_interrupt.h
 * Interrupt classes
 * System call interrupt
 */

#include "syscall_interrupt.h"
#include "../../syscall/syscalls/syscall_t.h"
#include "../../message/message.h"
#include <string>

static void msg_intr(std::string str)
{
	message::interrupt
		(message::wrap_core_info("kern syscall"))
		<< str
		<< message::msg_endl;
}

syscall_interrupt::syscall_interrupt ( syscall_t * _syscall )
	: internal_interrupt_t ( interrupt_id_t::SYSCALL )
	, syscall ( _syscall )
{
}

void syscall_interrupt::set_syscall ( syscall_t * _syscall )
{
	syscall = _syscall;
}

syscall_t * syscall_interrupt::get_syscall ()
{
	return syscall;
}

void syscall_interrupt::process ()
{
	msg_intr("Interrupt Service Routine start to serve syscall");
	int return_value = syscall->process ();
	syscall->set_return_value ( return_value );
}
