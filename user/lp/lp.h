#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"

class lp : public program {
public:

	lp();
	virtual ~lp();	
	virtual void static_init();
	virtual void main();
	
private:

	
};

void register_lp();
