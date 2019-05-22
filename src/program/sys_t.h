/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */
#pragma once
#include <string>
#include "../../dev/device_list.h"
#include "../../dev/devices/output/output.h"
#include "program.h"

class syscall_t;
class program;

class sys_t {
public:

	sys_t(program *prog);
	int create_process();
	int exec_program(int pid, handle<char> name);
	int yield();
	int exit();
    int wait(int pid);
	int read(int device);
	int write(dev_output *device, char data);
	int intr(syscall_t *sys);
	dev_output* std_output();

private:

	program *prog;
};

