#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include "state_info.h"
#include "intr.h"

using std::condition_variable;
using std::mutex;
using std::thread;
using std::unique_lock;
using std::cout;
using std::endl;

static void thread_main()
{
	intr_info info;
	info.intr_id = clock_intr;
	while (1) {
		sleep(1);
		intr_alert(&info);
	}
}

void clock_init()
{
	cout << "init clock." << endl;
	thread th(thread_main);
	th.join();
}
