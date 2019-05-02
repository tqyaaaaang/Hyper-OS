/*
 * Memory Manage Unit
 * Protection: when process access invalid address, trigger a #GP
 * Paging: when process access a page not loaded in memory, trigger a #PF
 * ---- Simulate ----
 * the process use CPU's interface read/write, which call this->mmu->read/write.
 * mmu get informations from current thread's status.
 */

#include "mmu.h"
#include <cstdint>
#include <cstddef>
#include "../mm/page_table.h"
#include "../interrupt/interrupts/intr_pagefault.h"
#include "../interrupt/interrupt.h"
#include "../status/status.h"

CPU_mmu::CPU_mmu()
{}

CPU_mmu::~CPU_mmu()
{}

CPU_mmu::CPU_mmu(CPU_core *core)
{
	this->core = core;
}

char CPU_mmu::read(size_t la)
{
	page_table *pg = status.get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	if (check(la, pte, false)) {

	} 
}

void CPU_mmu::write(size_t la, char c)
{
	page_table *pg = status.get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	if (check(la, pte, true)) {

	}
}

typedef intr_pagefault_t::error_info error_info;

// check if access is valid
// if not, trigger a #GP or #PF and return false
bool CPU_mmu::check(size_t la, pte_t *pte, bool write)
{
	int info = 0;
	if (!pte->user) {
		info |= intr_pagefault_t::E_SUPER;
	}
	if (!pte->write && write) {
		info |= intr_pagefault_t::E_WRITE;
	}
	if (!pte->present) {
		info |= intr_pagefault_t::E_PRESENT;
	}
	error_info einfo(la, info);
	while (interrupt(new intr_pagefault_t(einfo)) != 0);
	return true;
}
