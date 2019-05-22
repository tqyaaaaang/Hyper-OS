/**
 * apic/interrupt_queue.h
 * Queue of interrupts
 */

#pragma once

#include <queue>
#include <utility>
#include <functional>

class interrupt_t;

class interrupt_queue_t
{
public:
	interrupt_queue_t ();
	~interrupt_queue_t ();

	void push ( interrupt_t * current_interrupt );
	interrupt_t * front () const;
	void pop ();

	bool empty () const;
	void clear ();

private:
	typedef std::pair < std::pair < int, long long >, interrupt_t * > queue_element_t;   // format: ((priroity, time), interrupt)
	std::priority_queue < queue_element_t, std::vector < queue_element_t >, std::greater < queue_element_t > > interrupt_queue;

	long long time_id;
};
