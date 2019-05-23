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
	virtual handle<char> getchar();
	
private:

	program *prog;
	
};
