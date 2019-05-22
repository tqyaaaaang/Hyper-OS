#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"

class elephant : public program {
public:

	elephant();
	virtual ~elephant();
	virtual void main();
	virtual void static_init();
	
private:

};

void register_elephant();
