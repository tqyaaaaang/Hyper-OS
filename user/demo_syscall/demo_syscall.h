#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"

class demo_syscall : public program {
public:

	demo_syscall();
	virtual ~demo_syscall();
	virtual void static_init();
	virtual void main();
	
private:

	handle<char> hello;
};

void register_demo_syscall();
