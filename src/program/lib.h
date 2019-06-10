/**
 * program/lib.h
 * standard library for hos
 */

#pragma once

#include <string>
#include "program.h"

class hos_std_t {
public:

    hos_std_t(program *prog);
	virtual ~hos_std_t();
	virtual void println(std::string str);
	virtual void print(std::string str);
	virtual void println(handle<char> str);	
	virtual void print(handle<char> str);
	virtual void putchar(handle<char> c);
	virtual char getchar();
	virtual int read_int();
	virtual void write_int(int data);
	virtual void init();
	
private:

	program *prog;
	void write_recur(handle<int> dat);
	
};
