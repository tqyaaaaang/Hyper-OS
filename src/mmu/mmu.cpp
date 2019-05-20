/*
 * Memory Manage Unit
 * Protection: when process access invalid address, trigger a #GP
 * Paging: when process access a page not loaded in memory, trigger a #PF
 * ---- Simulate ----
 * process use CPU's interface read/write, which call this->mmu->read/write.
 * mmu get informations from core->context.
 */

#include "mmu.h"
#include <cstdint>
#include <cstddef>
#include "../mm/page_table.h"
#include "../interrupt/interrupts/intr_pagefault.h"
#include "../interrupt/interrupt.h"
#include "../status/status.h"
#include "../mm/pmem.h"
#include "../core/core.h"
#include "../logging/logging.h"

using logging::info;
using logging::log_endl;

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
	page_table *pg = core->get_context().get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	while (!check(la, pte, false)); // check passed
	pte->access = true;
    char result = pm::read(pte->paddr + (la % PAGE_SIZE));
	// logging::info << "MMU : " << result << " " << logging::log_endl;
	return result;
}

void CPU_mmu::write(size_t la, char c)
{
	page_table *pg = core->get_context().get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	while (!check(la, pte, true)); // check passed
	pte->access = true;
	pte->dirty = true;
	pm::write(pte->paddr + (la % PAGE_SIZE), c);
}

typedef intr_pagefault_t::error_info error_info;

// check if access is valid
// if not, trigger #PF 
bool CPU_mmu::check(size_t la, pte_t *pte, bool write)
{
	int info = 0;
	bool bug = false;
	if (!pte->present) {
		info |= intr_pagefault_t::E_PRESENT;
		bug = true;
	} else {
		if (!pte->user) {
			info |= intr_pagefault_t::E_SUPER;
			bug = true;
		}
		if (!pte->write && write) {
			info |= intr_pagefault_t::E_WRITE;
			bug = true;
		}
	}
	if (bug) {
		error_info einfo(la, info);
		interrupt(new intr_pagefault_t(einfo));
		return false;
		// todo check kill
	}
	return true;
}
