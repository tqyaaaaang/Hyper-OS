#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"
#include "../../src/program/program_manager.h"
#include <map>
#include <string>

class hyper_shell : public program {
public:

    hyper_shell();
	virtual ~hyper_shell();
	virtual void main();
	virtual void static_init();

	void register_program();
	
private:

	struct arg {
	    handle<char> opt;
		handle<char> val;
	};
	
	std::map<std::string> name_list;

	void help();
	void ls();
	void exec(handle<char> name);
};
