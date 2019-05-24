#pragma once
#include "../hyperstd.h"
#include "../hyperlib.h"

class demo_pagefault : public program {
public:

	demo_pagefault();
	virtual ~demo_pagefault();
	virtual void main();
	virtual void static_init();

private:

	
};

void register_demo_pagefault();
