/**
 * program/program_manager.h
 * find program by name
 */

#include "program_manager.h"
#include "../logging/logging.h"
#include <cstdlib>
#include <cstring>
#include <cassert>

using std::map;
using std::pair;
using std::string;
using std::make_pair;
using std::function;

map<string, program* (*)()> prog_table;

void register_program(string name, program* (*gen)() )
{
	assert(!prog_table.count(name)); 
	prog_table[name] = gen;
}

program* get_program(string name)
{
	assert(prog_table.count(name));
	program* (*gen)() = prog_table[name];
	program* prog = gen();
	prog->build();
	return prog;
}

void destroy_program_manager()
{
}
