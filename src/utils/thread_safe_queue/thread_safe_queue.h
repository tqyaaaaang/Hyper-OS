/**
 * utils/thread_safe_queue/thread_safe_queue.h
 * thread-safe queue
 */

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template < typename T >
class thread_safe_queue
{
public:
	void push ( T t );
	T pop ();

	bool empty ();

	void notify ();

private:
	std::queue < T > q;
	mutable std::mutex mut;
	std::condition_variable cv;
};
