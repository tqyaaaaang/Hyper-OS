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
#include "../src/syscall/syscalls/sys_read.h"
#include "../src/syscall/syscalls/sys_write.h"
#include "../dev/device_list.h"

void test_syscall ()
{
	logging::debug << "Testing system calls" << logging::log_endl;
	int return_value;

	status.get_core ()->acquire ();

	{
		syscall_test current_syscall;
		return_value = syscall ( &current_syscall );
		logging::info << "Test on SYSCALL TEST system call succeeded, interrupt return value : " << return_value << ", syscall return value : " << current_syscall.get_return_value () << logging::log_endl;
	}

	//{
	//	sys_read read_syscall ( device_desc::standard_input );
	//	return_value = syscall ( &read_syscall );
	//	logging::info << "Test on SYSCALL READ succeeded, interrupt return value : " << return_value << ", syscall return value : " << read_syscall.get_return_value () << logging::log_endl;

	//	sys_write write_syscall ( device_desc::standard_output, read_syscall.get_return_value () );
	//	return_value = syscall ( &write_syscall );
	//	logging::info << "Test on SYSCALL WRITE succeeded, interrupt return value : " << return_value << ", syscall return value : " << write_syscall.get_return_value () << logging::log_endl;
	//}

	status.get_core ()->release ();
}
