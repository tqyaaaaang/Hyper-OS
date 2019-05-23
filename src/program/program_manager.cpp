/**
 * program/program_manager.h
 * find program by name
 */

#include "program_manager.h"
#include "../logging/logging.h"
#include "../../user/shell/shell.h"
#include "../../user/lp/lp.h"
#include "../../user/elephant/elephant.h"
#include "../../user/matrix/matrix.h"
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>

using std::vector;
using std::map;
using std::pair;
using std::string;
using std::make_pair;
using std::function;

map<string, program* (*)()> prog_table;
vector<string> prog_name;

void register_program(string name, program* (*gen)() )
{
	assert(!prog_table.count(name)); 
	prog_table[name] = gen;
	prog_name.push_back(name);
}

bool is_program(string name)
{
	return prog_table.count(name);
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

void init_program_manager()
{
	register_shell();
	register_lp();
	register_elephant();
	register_matrix();
}
