/**
 * program/alloc
 * alloc on data/bss/stack/heap
 */

#include "program.h"
#include <cstdlib>
#include <cassert>
#include "../utils/panic.h"

using handle_type::type;

template<typename T> 
type handle<T>::get_type() const
{
	return this_type;
}

template<typename T>
void handle<T>::set_type(type t)
{
	this_type = t;
}

template<typename T>
handle<T> program::alloc_static()
{
	
}

template<typename T>
handle<T> program::alloc_static(size_t n)
{
	
}

template<typename T>
handle<T> program::alloc_bss()
{
	
}

template<typename T>
handle<T> program::alloc_bss(size_t n)
{
	
}
