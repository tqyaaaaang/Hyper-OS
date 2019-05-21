/**
 * program/program_manager.h
 * find program by name
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "program.h"

extern std::map<std::string,
	std::pair<size_t, program*> > prog_table;

/**
 * register a program
 * @name : name of program
 * @prog : program to register
 * @size : sizeof(prog)
 */
void register_program(std::string name, program *prog, size_t size);

program* get_program(std::string name);

void destroy_program_manager();
