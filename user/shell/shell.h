#pragma once

#include "../hyperstd.h"
#include "../hyperlib.h"
#include "../../src/logging/logging.h"
#include <string>

class hyper_shell : public program {
public:

    hyper_shell();
	virtual ~hyper_shell();
	virtual void main();
	virtual void static_init();

private:

	handle<int> argc;
	handle<char> argv[20];

    handle<int> servant(handle<char> cmd);
	void exec();
	handle<int> parse();
	handle<int> get();

	void program_not_found_error();
	void too_long_error();
	void format_error();
	void argv_error(handle<int> id);
	
	std::string to_string(handle<char> str);

	void help_prog();
	void msg_prog();
	
	void general_help();
	
	void help_exit_title();
	void help_exec_title();
	void help_msg_title();

	void help_msg_body();
};

void register_shell();
