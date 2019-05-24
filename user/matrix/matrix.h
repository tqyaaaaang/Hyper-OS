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

    void hint(handle<int> n, handle<int> m);
	void read_matrix(handle<int> n, handle<int> m, handle< handle<int> > &A);
	void write_matrix(handle<int> n, handle<int> m, handle< handle<int> > A);
	void alloc_matrix(handle<int> n, handle<int> m, handle< handle<int> > &A);

	handle<int> at(handle< handle<int> > A, handle<int> i, handle<int> j);
	void matrix_mul(handle< handle<int> > &C, handle< handle<int> > A, handle< handle<int> > B, handle<int> n, handle<int> m, handle<int> q);
};

void register_matrix();
