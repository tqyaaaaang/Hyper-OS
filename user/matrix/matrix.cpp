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
{
	
}

matrix::~matrix()
{
	
}

void matrix::main()
{
	
}

void matrix::static_init()
{
	
}


