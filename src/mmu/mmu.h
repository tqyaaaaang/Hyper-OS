/*
* Memory Manage Unit
* Protection: when process access invalid address, trigger a #GP
* Paging: when process access a page not loaded in memory, trigger a #PF
*/
#pragma once
#include <cstdint>
#include <cstddef>
#include "../core/core.h"

class CPU_core;

/**
* class CPU_mmu
* Each instance of this class is a mmu of a core
*/
class CPU_mmu {
public:
	
	char read ( size_t la );
	void write ( size_t la, char data );
private:

	CPU_core *self_core;
};
