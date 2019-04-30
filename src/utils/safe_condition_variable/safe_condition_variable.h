/**
 * utils/safe_condition_variable/safe_condition_variable.h
 * Safe condition variable
 * which can save notifications if no threads are waiting
 */

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class safe_condition_variable
{
public:
	void notify ()
	{
		std::unique_lock < std::mutex > lock ( mut );
		--count;
		if ( count >= 0 ) {   // There are threads waiting
			cv.notify_one ();
		}
	}

	void wait ()
	{
		std::unique_lock < std::mutex > lock ( mut );
		++count;
		if ( count > 0 ) {   // There aren't any spare notifications
			cv.wait ( lock );
		}
	}

private:
	std::mutex mut;   // lock
	std::condition_variable cv;   // condition variable
	int count;   // waiting count of the condition variable, < 0 if there are spare notifications, > 0 if there are waiting threads
};
