/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */
#pragma once
#include <string>
#include "../../dev/device_list.h"
#include "../../dev/devices/output/output.h"

class sys_t {
public:

	int create_process();
	int exec_program(int pid, const std::string &name);
	int yield();
	int exit();
    int wait(int pid);
	int read(int device);
	int write(dev_output *device, char data);

	dev_output* std_output();
	
};

