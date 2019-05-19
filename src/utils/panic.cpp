#include "panic.h"
#include <string>
using std::string;

void panic(const string &str)
{
	logging::debug << "System Panic. info = " << str << logging::log_endl;
}

void panic()
{
	panic("null");
}
