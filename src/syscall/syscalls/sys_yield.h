/**
 * syscall/syscalls/sys_yield.h
 * create_proc syscall
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_yield : public syscall_t {
public:

    sys_yield();
    virtual int process();
};

