/**
 * (l)ist all (p)rograms
 * list all programs
 */
#include "lp.h"
#include "../../src/program/program_manager.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::vector;

extern vector<string> prog_name;

static program* gen()
{
	return new lp();
}

void register_lp()
{
	register_program("lp", gen);
}

lp::lp()
{ }

lp::~lp()
{ }

void lp::static_init()
{ }

void lp::main()
{
	handle<int> i = 0;
	handle<int> size = (int)prog_name.size();
	for (i = 0; i < size; i = i + 1) {
		hos_std->println(prog_name[i]);
	}
}
