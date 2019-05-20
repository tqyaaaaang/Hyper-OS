/**
 * syscall/syscalls/sys_yield.h
 * create_proc syscall
 */
#pragma once

#include "../../process/process.h"

class sys_yield {
public:

    sys_yield();
    virtual int process();
};

