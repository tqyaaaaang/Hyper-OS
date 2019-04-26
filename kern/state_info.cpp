#include <iostream>
#include "state_info.h"
#include "defs.h"
#include "process.h"

using std::cout;
using std::endl;

process *current_proc;
size_t next_pid;
page_dir *pgdir;

void state_init()
{
	cout << "init state." << endl;
	current_proc = NULL;
	pgdir = NULL;
	next_pid = user_proc_start;
}
