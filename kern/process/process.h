#pragma once

#include <thread>
#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <list>
#include "page_dir.h"
#include "defs.h"
#include "state_info.h"

class process {

public:
	size_t proc_id;                 // pid
	int state;                      // state. def in defs.h
	std::thread th;                 // thread of the proccess
	std::condition_variable condvar;// condvar to sleep on
	std::mutex mut;                 // mutex for condvar
	page_dir *pgdir;                // page dir of the proccess
	int time_slice;                 // remained time slice 

	process();
	void awake_proc();             // awake a sleeping proc
	void load_proc(std::function<void()>);          // load a proc
};

extern std::list<process> runnable, sleeping;

void schedule();

