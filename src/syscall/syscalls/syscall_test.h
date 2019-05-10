/**
 * syscall/syscalls/syscall_test.h
 * System call classes
 * System call test
 */

#pragma once

#include "syscall_t.h"

class syscall_test : public syscall_t
{
public:
	syscall_test ();

	virtual int process ();
};
