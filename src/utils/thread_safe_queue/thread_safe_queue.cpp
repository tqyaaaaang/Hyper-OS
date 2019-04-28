/**
 * utils/thread_safe_queue/thread_safe_queue.h
 * thread-safe queue
 */

#include "thread_safe_queue.h"

template<typename T>
void thread_safe_queue<T>::push ( T t )
{
	std::unique_lock < std::mutex > lock ( mut );
	q.push ( t );
	cv.notify_one ();
}

template<typename T>
T thread_safe_queue<T>::pop ()
{
	std::unique_lock < std::mutex > lock ( mut );
	if ( q.empty () ) {
		cv.wait ( lock );
		if ( q.empty () ) {
			return T ();
		}
	}
	T return_value = q.front ();
	q.pop ();
	return return_value;
}

template<typename T>
bool thread_safe_queue<T>::empty ()
{
	std::unique_lock < std::mutex > lock ( mut );
	return q.empty ();
}

template<typename T>
void thread_safe_queue<T>::notify ()
{
	std::unique_lock < std::mutex > lock ( mut );
	cv.notify_one ();
}
