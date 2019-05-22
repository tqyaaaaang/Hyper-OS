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
#include <functional>
#include "program.h"

/**
 * register a program
 * @name : name of program
 * @gen  : program generator
 */
void register_program(std::string name,
					  program* (*gen)());

program* get_program(std::string name);

void destroy_program_manager();
