#include "matrix.h"
#include "../../src/program/program_manager.h"

program* gen()
{
	return new matrix;
}

void register_matrix()
{
	register_program("matrix", gen);
}

matrix::matrix()
{ }

matrix::~matrix()
{ }

void matrix::static_init()
{ }

void matrix::hint(handle<int> n, handle<int> m)
{
	hos_std->print("input a ");
	hos_std->write_int(n);
	hos_std->print(" x ");
	hos_std->write_int(m);
	hos_std->println(" matrix");
}

handle<int> matrix::at(handle< handle<int> > A, handle<int> i, handle<int> j)
{
	return ((handle<int>)A[i])[j];
}

void matrix::read_matrix(handle<int> n, handle<int> m,
						 handle< handle<int> > &A)
{
	for (handle<int> i = 0; i < n; i = i + 1) {
		for (handle<int> j = 0; j < m; j = j + 1) {
			at(A, i, j) = hos_std->read_int();
		}
	}
}

void matrix::write_matrix(handle<int> n, handle<int> m,
						  handle< handle<int> > A)
{
	for (handle<int> i = 0; i < n; i = i + 1) {
		for (handle<int> j = 0; j < m; j = j + 1) {
			hos_std->write_int(at(A, i, j));
			hos_std->print(" ");
		}
		hos_std->println("");
	}
}

void matrix::alloc_matrix(handle<int> n, handle<int> m,
						  handle< handle<int> > &A)
{
	A = alloc_heap< handle<int> >( (int)n );
    for (handle<int> i = 0; i < n; i = i + 1) {
	    A[i] = alloc_heap<int> (m);
		for (handle<int> j = 0; j < m; j = j + 1) {
			at(A, i, j) = alloc_heap<int>();
		}
	}
}

void matrix::matrix_mul(handle< handle<int> > &C, handle< handle<int> > A, handle< handle<int> > B, handle<int> n, handle<int> m, handle<int> q)
{
	for (handle<int> i = 0; i < n; i = i + 1)
		for (handle<int> j = 0; j < q; j = j + 1)
			at(C, i, j) = 0;
	for (handle<int> k = 0; k < m; k = k + 1) {
		for (handle<int> i = 0; i < n; i = i + 1) {
			for (handle<int> j = 0; j < q; j = j + 1) {
				at(C, i, j) = at(C, i, j) + at(A, i, k) * at(B, k, j);
			}
		}
	}
}

void matrix::main()
{
	handle< handle<int> > A, B, C;
	hos_std->println("input matrix size : n, m, k");
	handle<int> n = hos_std->read_int();
	handle<int> m = hos_std->read_int();
	handle<int> k = hos_std->read_int();
	hint(n, m);
	alloc_matrix(n, m, A);
	read_matrix(n, m, A);
	hint(m, k);
	alloc_matrix(m, k, B);
	read_matrix(m, k, B);
	alloc_matrix(n, k, C);
    matrix_mul(C, A, B, n, m, k);
	hos_std->println("result matrix");
	write_matrix(n, k, C);
	hos_std->print("computation finished in : ");
	hos_std->write_int(sys->clock() * CLOCK_INTERVAL);
	hos_std->println(" ms.");
}

