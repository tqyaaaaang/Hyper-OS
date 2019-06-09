/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */
#pragma once
#include <string>
#include "program.h"

class syscall_t;
class program;
class dev_output;
class dev_input;

class sys_t {
public:

	sys_t(program *prog);
	int create_process();
	int exec_program(int pid, handle<char> name);
	int yield();
	int exit();
	int wait(int pid);
	int read(dev_input *device);
	int write(dev_output *device, char data);
	int intr(syscall_t *sys);
	dev_output* std_output();
	dev_input* std_input();
	int pid();
	
private:

	program *prog;
};

