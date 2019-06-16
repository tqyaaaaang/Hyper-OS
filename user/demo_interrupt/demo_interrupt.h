#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"

class demo_interrupt : public program {
public:

	demo_interrupt();
	virtual ~demo_interrupt();
	virtual void static_init();
	virtual void main();
	
private:

	handle<char> hello;
};

void register_demo_syscall();
