/**
 * syscall/syscalls/sys_write.h
 * write to device syscall
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"
#include "../../../dev/device_list.h"
#include "../../../dev/devices/output/output.h"

class sys_write : public syscall_t {
public:

    sys_write(dev_output *device, char data);
    virtual int process();

private:

	dev_output *device;
	char data;
};

