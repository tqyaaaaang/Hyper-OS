/**
 * program/lib.h
 * standard library for hos
 */

#include "lib.h"
#include "sys_t.h"
#include "program.h"
#include "../logging/logging.h"
#include "../message/message.h"

using std::string;
using message::wrap_core_info;

static void msg_intr(string str)
{
	message::interrupt(wrap_core_info("user stdlib"))
		<< str
		<< message::msg_endl;
}

hos_std_t::hos_std_t(program *prog)
{
	this->prog = prog;
}

hos_std_t::~hos_std_t()
{
	
}

void hos_std_t::init()
{
	
}

void hos_std_t::println(string str)
{
	msg_intr("call function \'println\' in standard library");
	sys_t *sys = this->prog->sys;
	for (char i : str) {
	    sys->write(sys->std_output(), i);
	}
	sys->write(sys->std_output(), '\n');
}

void hos_std_t::print(string str)
{
	msg_intr("call function \'print\' in standard library");
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
	msg_intr("call function \'print\' in standard library");
	for (size_t i = 0; str[i] != '\0'; i++) {
		this->putchar(str[i]);
	}
}

void hos_std_t::println(handle<char> str)
{
	msg_intr("call function \'println\' in standard library");
	for (size_t i = 0; str[i] != '\0'; i++) {
		this->putchar(str[i]);
	}
	this->putchar('\n');
}

int hos_std_t::getchar()
{
	sys_t *sys = prog->sys;
	handle<int> data = sys->read(sys->std_input());
	return (int)data;
}

int hos_std_t::read_int()
{
	msg_intr("call function \'read_int\' in standard library");
	handle<int> result = 0;
	handle<int> ch = (int)0;
	handle<int> flag = 1;
	while ((ch < '0' || ch > '9') && ch != '-') {
		ch = this->getchar();
	}
	if (ch == '-') {
		flag = -1;
		ch = this->getchar();
	}
	do {
		result = result * 10 + (int)(ch - '0');
		ch = this->getchar();
	} while ('0' <= ch && ch <= '9');
	int dat = result * flag;
	logging::debug << "read a int : " << dat << logging::log_endl;
	return result * flag;
}

void hos_std_t::write_recur(handle<int> dat)
{
	if (dat != 0) {
		write_recur(dat / 10);
		msg_intr("call function \'print\' in standard library");
		this->putchar((int)((dat % 10) + '0'));
	}
}

void hos_std_t::write_int(int data)
{
	msg_intr("call function \'write_int\' in standard library");
	if (data < 0) {
		this->putchar('-');
		data = -data;
	}
	handle<int> num = data;
	if (num == 0) {
		this->putchar('0');
	} else {
		write_recur((int)num);
	}
	logging::debug << "write a int : " << num << logging::log_endl;
}

