#include "state_info.h"
#include "defs.h"
#include "process.h"
#include <iostream>

using std::cout;
using std::endl;

process *current_proc;
size_t next_pid;
	
void state_init()
{
	cout << "init state." << endl;
	current_proc = NULL;
	next_pid = user_proc_start;
}
