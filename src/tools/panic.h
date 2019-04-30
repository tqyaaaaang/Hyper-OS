/*
 * System panic
 */
#pragma once
#include "../logging/logging.h"
#include <string>

void panic(const std::string &panic_info);
void panic();

