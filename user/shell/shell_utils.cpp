#include "shell.h"

using std::string;

string hyper_shell::to_string(handle<char> str)
{
	string tar;
	for (size_t i = 0; str[i] != '\0'; i++) {
		tar = tar + (char)(str[i]);
	}
	return tar;
}

void hyper_shell::program_not_found_error()
{
	hos_std->print("invalid program name \'");
	hos_std->print(argv[0]);
	hos_std->println("\'");
}

void hyper_shell::too_long_error()
{
	hos_std->println("command is too long");
}
