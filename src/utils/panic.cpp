#include "panic.h"
#include <cstdlib>
#include <string>
using std::string;

void panic(const string &str)
{
	logging::debug << "System Panic. info = " << str << logging::log_endl;
	exit(1);
}

void panic()
{
	panic("null");
}
