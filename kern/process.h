#pragma once

#include <thread>
#include <cstdint>
#include <condition_variable>
#include "page_dir.h"
#include "defs.h"

using std::thread;
using std::condition_variable;

class process {

public:
	size_t proc_id;            // pid
	int state;                 // state. def in defs.h
	thread th;                 // thread of the proccess
	condition_variable condvar;// condvar to sleep on 
	page_dir *pgdir;           // page dir of the proccess

	void awake_proc();         // awake a sleeping proc
};
