/**
 * program/program_manager.h
 * find program by name
 */

#include "program_manager.h"
#include <cstdlib>
#include <cstring>
#include <cassert>

using std::map;
using std::pair;
using std::string;
using std::make_pair;

map<string,
	pair<size_t, program*> > prog_table;

void register_program(string name, program *prog, size_t size)
{
	assert(!prog_table.count(name)); 
	prog_table[name] = make_pair(size, prog);
}

program* get_program(string name)
{
	assert(prog_table.count(name));
	size_t size = prog_table[name].first;
	program *prog = prog_table[name].second;
	char *new_prog = (char*)malloc(size);
	memcpy(new_prog, prog, size);
	return (program*)new_prog;
}
