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
	void push_back ( T t );
	void push_front ( T t );
	T pop_back ();
	T pop_front ();

	bool empty ();

	void clear ();

private:
	std::deque < T > q;
	mutable std::mutex mut;
	std::condition_variable cv;
};



template<typename T>
void thread_safe_queue<T>::push_back ( T t )
{
	std::unique_lock < std::mutex > lock ( mut );
	q.push_back ( t );
	cv.notify_one ();
}

template<typename T>
void thread_safe_queue<T>::push_front ( T t )
{
	std::unique_lock < std::mutex > lock ( mut );
	q.push_front ( t );
	cv.notify_one ();
}

template<typename T>
T thread_safe_queue<T>::pop_back ()
{
	std::unique_lock < std::mutex > lock ( mut );
	while ( q.empty () ) {
		cv.wait ( lock );
	}
	T return_value = q.back ();
	q.pop_back ();
	return return_value;
}

template<typename T>
T thread_safe_queue<T>::pop_front ()
{
	std::unique_lock < std::mutex > lock ( mut );
	while ( q.empty () ) {
		cv.wait ( lock );
	}
	T return_value = q.front ();
	q.pop_front ();
	return return_value;
}

template<typename T>
bool thread_safe_queue<T>::empty ()
{
	std::unique_lock < std::mutex > lock ( mut );
	return q.empty ();
}

template<typename T>
inline void thread_safe_queue<T>::clear ()
{
	std::unique_lock < std::mutex > lock ( mut );
	cv.notify_all ();
	q.clear ();
}
