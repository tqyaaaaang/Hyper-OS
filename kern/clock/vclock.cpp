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

mutex ticker;

static void thread_main()
{
	intr_info info;
	info.intr_id = clock_intr;
	while (1) {
		sleep(1);
		ticker.lock();
		cout << "ticker 1" << endl;
		intr_alert(&info);
		ticker.unlock();
	}
}

static void thread_main_two()
{
	intr_info info;
	info.intr_id = clock_intr;
	while (1) {
		sleep(2);
		ticker.lock();
		cout << "ticker 2" << endl;
		intr_alert(&info);
		ticker.unlock();
	}
}

void clock_init()
{
	cout << "init clock." << endl;
	thread th(thread_main), th2(thread_main_two);
	th.join();
	th2.join();
}
