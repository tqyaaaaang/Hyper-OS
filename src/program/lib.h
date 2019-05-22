/**
 * program/lib.h
 * standard library for hos
 */

#pragma once

#include <string>

class program;

class hos_std_t {
public:

    hos_std_t(program *prog);
	virtual ~hos_std_t();
	virtual void println(std::string str);
	virtual void print(std::string str);
    
private:

	program *prog;
	
};
