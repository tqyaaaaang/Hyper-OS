#pragma once
#include "../hyperstd.h"
#include "../hyperlib.h"

class demo_process : public program {
public:

	demo_process();
	virtual ~demo_process();
	virtual void main();
	virtual void static_init();

private:

	handle<char> name;
};

void register_demo_process();
