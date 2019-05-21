/**
 * syscall/syscalls/sys_create_proc.h
 * create_proc syscall
 */
#pragma once

#include "syscall_t.h"
#include "../../process/process.h"

class sys_create_proc : public syscall_t {
public:

	sys_create_proc();
    virtual int process();
};

