#include "sys_exit.h"

sys_exit::sys_exit()
	:syscall_t(syscall_id_t::EXIT)
{}

int sys_exit::process()
{
	return proc_exit();
}
