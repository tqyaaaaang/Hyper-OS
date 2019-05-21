/**
 * syscall/syscalls/sys_exec_prog.h
 * create_proc syscall
 */
#pragma once

#include "syscall_t.h"
#include "../../process/process.h"

class sys_exec_prog : public syscall_t {
public:

	sys_exec_prog(size_t pid, program *prog);
    virtual int process();

private:
	size_t pid;
	program *prog;
};

