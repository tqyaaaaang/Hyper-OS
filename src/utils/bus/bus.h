#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

template<typename T>
class bus {
public:

    T read();
	void write(const T&);
	bool empty();
	
private:
	std::queue<T> q;
	mutable std::mutex mutex_lock;
};

template<typename T>
bool bus<T>::empty()
{
	std::lock_guard<std::mutex> locker(mutex_lock);
	return q.empty();
}

template<typename T>
T bus<T>::read()
{
	while (empty()) 
		std::this_thread::yield();
	mutex_lock.lock();
	T cur = q.front();
	q.pop();
	mutex_lock.unlock();
	return cur;
}

template<typename T>
void bus<T>::write(const T &data)
{
	std::lock_guard<std::mutex> locker(mutex_lock);
	q.push(data);
}


