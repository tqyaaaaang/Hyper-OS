/**
 * test/test_syscall.cpp
 * Test system call
 */

#include "test_syscall.h"

#include "../src/logging/logging.h"
#include "../src/core/core.h"
#include "../src/status/status.h"
#include "../src/syscall/syscall.h"
#include "../src/syscall/syscalls/syscall_test.h"

void test_syscall ()
{
	logging::debug << "Testing interrupts" << logging::log_endl;
	int return_value;

	status.get_core ()->acquire ();

	{
		syscall_test current_syscall;
		return_value = syscall ( &current_syscall );
		logging::info << "Test on SYSCALL TEST system call succeeded, interrupt return value : " << return_value << ", syscall return value : " << current_syscall.get_return_value () << logging::log_endl;
	}

	status.get_core ()->release ();
}
