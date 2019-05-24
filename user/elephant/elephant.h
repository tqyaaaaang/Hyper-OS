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

	handle<char> graph;
	handle<char> elep;
};

void register_elephant();
