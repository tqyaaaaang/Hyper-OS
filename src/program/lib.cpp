/**
 * program/lib.h
 * standard library for hos
 */

#include "lib.h"
#include "sys_t.h"
#include "program.h"

using std::string;

hos_std_t::hos_std_t(program *prog)
{
	this->prog = prog;
}

hos_std_t::~hos_std_t()
{
	
}

void hos_std_t::println(string str)
{
	sys_t *sys = this->prog->sys;
	for (char i : str) {
	    sys->write(sys->std_output(), i);
	}
	sys->write(sys->std_output(), '\n');
}

void hos_std_t::print(string str)
{
	sys_t *sys = this->prog->sys;
	for (char i : str) {
	    sys->write(sys->std_output(), i);
	}
}

void hos_std_t::putchar(handle<char> c)
{
	std::putchar(c);
}

void hos_std_t::println(handle<char> str)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		putchar(str[i]);
	}
	println("");
}

