/**
 * program/lib.h
 * standard library for hos
 */

#include "lib.h"
#include "sys_t.h"
#include "program.h"
#include "../logging/logging.h"

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
	sys_t *sys = prog->sys;
    sys->write(sys->std_output(), (char)c);
}

void hos_std_t::print(handle<char> str)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		this->putchar(str[i]);
	}
}

void hos_std_t::println(handle<char> str)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		this->putchar(str[i]);
	}
	println("");
}

char hos_std_t::getchar()
{
	sys_t *sys = prog->sys;
	return sys->read(sys->std_input());
}

int hos_std_t::read_int()
{
	handle<int> result = 0;
	handle<char> ch = (char)0;
	while (ch < '0' || ch > '9') {
		ch = this->getchar();
	}
	do {
		result = result * 10 + (int)(ch - '0');
		ch = this->getchar();
	} while ('0' <= ch && ch <= '9');
	int dat = result;
	logging::debug << "read a int : " << dat << logging::log_endl;
	return result;
}

void hos_std_t::write_recur(handle<int> dat)
{
	if (dat != 0) {
		write_recur(dat / 10);
		this->putchar(char(dat % 10) + '0');
	}
}

void hos_std_t::write_int(int data)
{
	handle<int> num = data;
	if (num == 0) {
		this->print("0");
	} else {
		write_recur((int)num);
	}
	logging::debug << "write a int : " << num << logging::log_endl;
}

