/**
 * syscall/syscall.cpp
 * System call entry
 */

#include "syscall.h"
#include "../interrupt/interrupt.h"
#include "../interrupt/interrupts/syscall_interrupt.h"

int syscall ( syscall_t * current_syscall )
{
	syscall_interrupt current_interrupt ( current_syscall );
	return interrupt ( &current_interrupt );
}
