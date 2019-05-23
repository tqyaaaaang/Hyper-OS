#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"

class matrix : public program {
public:

	matrix();
	virtual ~matrix();
	virtual void main();
	virtual void static_init();
private:

	handle<handle<int> > A;
};

void register_matrix();
