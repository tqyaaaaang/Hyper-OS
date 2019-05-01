/*
 * First Fit memory allocation
 */

#include "allocator.h"
#include "ffma.h"
#include <list>
#include <cassert>
#include "../panic.h"
#include "../../logging/logging.h"

using std::list;
using logging::debug;
using logging::log_endl;

typedef allocator::segment segment;

ffma::ffma(size_t n)
{
	assert(n > 0);
	set_size(n);
	free_list.push_back(segment(0, n));
}

size_t ffma::malloc(size_t len)
{
	assert(len > 0);
	auto pos = free_list.end();
	for (list<segment>::iterator i = free_list.begin();
		 i != free_list.end(); i++) {
		if (i->length >= len) {
			pos = i;
			break;
		}
	}
	if (pos == free_list.end()) {
		panic("memory allocation failed");
	}
	if (pos->length == len) {
		size_t start = pos->start;
		free_list.erase(pos);
		return start;
	} else {
		size_t start = pos->start;
		segment remained(start + len, pos->length - len);
		auto nxt = pos; 
		free_list.insert(++nxt, remained);
		free_list.erase(pos);
		return start;
	}
}

void ffma::merge(list<segment>::iterator pos)
{
	if (pos != free_list.begin()
		&& pos != free_list.end()) {
		auto bef = pos;
		bef--;
		if (bef->start + bef->length == pos->start) {
			bef->length += pos->length;
			free_list.erase(pos);
		}
	}
}

void ffma::free(size_t addr, size_t len)
{
	assert(len > 0);
	auto pos = free_list.end();
	segment seg(addr, len);
	for (auto i = free_list.begin(); i != free_list.end(); i++) 
		if (i->start >= addr) {
			pos = i;
			break;
		}
	free_list.insert(pos, seg);
	merge(pos);
}

