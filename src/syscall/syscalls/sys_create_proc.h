/**
 * syscall/syscalls/sys_create_proc.h
 * create_proc syscall
 */
#pragma once

#include "../../process/process.h"

class sys_create_proc {
public:

	sys_create_proc();
    virtual int process();
};

