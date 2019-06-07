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
#include <string>
#include "../mm/page_table.h"
#include "../interrupt/interrupts/intr_pagefault.h"
#include "../interrupt/interrupt.h"
#include "../status/status.h"
#include "../mm/pmem.h"
#include "../core/core.h"
#include "../logging/logging.h"
#include "../message/message.h"

using logging::info;
using logging::log_endl;
using std::string;

static void msg_mm(string str)
{
	message::memory(message::wrap_core_info("hd MMU"))
		<< str
		<< message::msg_endl;
}

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
	message::memory(message::wrap_core_info("hd MMU"))
		<< "memory read operation at linear address "
		<< la 
		<< message::msg_endl; 
	page_table *pg = core->get_context().get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	while (!check(la, pte, false)) {
		msg_mm("page fault ISR finish, run the memory access instruction again");
	}
	// check passed
	message::memory(message::wrap_core_info("hd MMU"))
		<< "MMU translate linear address "
		<< la
		<< " to physical address "
		<< "(page frame address)" << pte->paddr
		<< " + (offset)" << la % PAGE_SIZE
		<< " = " << pte->paddr + (la % PAGE_SIZE)
		<< message::msg_endl; 
	pte->access = true;
	char result = pm::read(pte->paddr + (la % PAGE_SIZE));
	logging::info << "MMU : read " << result << " " << logging::log_endl;
	return result;
}

void CPU_mmu::write(size_t la, char c)
{
	message::memory(message::wrap_core_info("hd MMU"))
		<< "memory write operation at linear address : "
		<< la 
		<< message::msg_endl;
	page_table *pg = core->get_context().get_page_table();
	pte_t *pte = pg->get_pte_try(la);
	while (!check(la, pte, true)) {
		msg_mm("page fault ISR finish, run the memory access instruction again");
	}
	// check passed
	message::memory(message::wrap_core_info("hd MMU"))
		<< "MMU translate linear address : "
		<< la
		<< " to physical address "
		<< "(page frame address)" << pte->paddr
		<< " + (offset)" << la % PAGE_SIZE
		<< " = " << pte->paddr + (la % PAGE_SIZE)
		<< message::msg_endl; 
	pte->access = true;
	pte->dirty = true;
	pm::write(pte->paddr + (la % PAGE_SIZE), c);
	logging::info << "MMU : write " << c << " " << logging::log_endl;
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
		msg_mm((write ? string("(write)") : string("(read)"))
			   + " mmu check fail, page is "
			   + (pte->present ? string("present") : string("not present"))
			   + ", "
			   + (pte->user ? string("available") : string("unavailable"))
			   + " for user and "
			   + (pte->write ? string("writable.") : string("unwritable")));
		error_info einfo(la, info);
		interrupt(new intr_pagefault_t(einfo));
		return false;
		// todo check kill
	} else {
		msg_mm((write ? string("(write)") : string("(read)"))
			   + " mmu check pass");
	}
	return true;
}
