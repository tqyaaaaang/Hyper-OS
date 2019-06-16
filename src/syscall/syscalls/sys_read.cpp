/**
 * syscall/syscalls/sys_read.h
 * read from device syscall
 */

#include "sys_read.h"
#include "../../logging/logging.h"
#include "../../../dev/device_list.h"

sys_read::sys_read ( dev_input * device )
	: syscall_t ( syscall_id_t::READ )
	, device ( device )
{
}

int sys_read::process ()
{
	logging::debug << "Processing syscall READ" << logging::log_endl;
	device_desc::standard_input->read ();
	return 0;
}
