/**
 * interrupt/interrupts/syscall_interrupt.h
 * Interrupt classes
 * System call interrupt
 */

#pragma once

#include "internal_interrupt_t.h"

class syscall_t;

class syscall_interrupt : public internal_interrupt_t
{
public:
	syscall_interrupt ( syscall_t *_syscall );

	void set_syscall ( syscall_t *_syscall );
	syscall_t * get_syscall ();

	virtual void process ();

private:
	syscall_t *syscall;
};
