/*
 * Abstract type for Virtual OS
 * Send operations to MMU & detect interrupt
 */

#pragma once
#include <cstdint>

template<typename T>
class abstract_type {
	void intr_hook();
public:
	size_t va;
	abstract_type();
	abstract_type(size_t va);
	abstract_type& operator = (const T &val);
	abstract_type& operator = (const abstract_type &val);
	operator T() const ;
};

