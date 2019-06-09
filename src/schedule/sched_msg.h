#pragma once

#include "../message/message.h"
#include <string>

class process_t;

void msg_proc(std::string str);
std::string to_string(int dat);
std::string proc_info(process_t *proc);
std::string proc_state(process_t *proc);
