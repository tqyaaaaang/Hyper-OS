/**
 * syscall/syscalls/syscall_test.h
 * System call classes
 * System call test
 */

#include "syscall_test.h"
#include "../../logging/logging.h"

syscall_test::syscall_test ()
	: syscall_t ( syscall_id_t::SYSCALL_TEST )
{
}

int syscall_test::process ()
{
	logging::info << "Processing system call TEST" << logging::log_endl;
	return 31415926;
}
