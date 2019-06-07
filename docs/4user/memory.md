# Memory and Memory Manager

## Physical Memory

Hyper OS considers physical memory as a special hardware. Like other hardware, physical memory runs on a **single thread** (with name `memory` in `status.name`), waiting for memory access instructions of MMU. Main function of memory thread is in `src/mm/pmem.cpp, memory_main()`.

MMU use the interface `pm::read` and `pm::write`, which will send a signal (`pm_info`) to physical memory thread and waiting for result (`pm_result`). 

- `size_t read(size_t paddr)`: read a byte from physical address `@paddr`, and return the result.
- `void write(size_t paddr, char data)`: write a byte `@data` to physical address `@paddr`.

After receiving signal from `MMU`, physical memory simulator calls functions `pm_read` and `pm_write`.

In order to reduce demand of memory, physical memory of Hyper OS allocate memory dynamically. Physical memory is divided into page frames in size of 4KB, only visited pages are actually allocated.

## Memory Manage Unit

Memory manage unit (MMU) is the key to paging. It provides interface of virtual memory read/write, translating the address to physical address using page table and access physical memory. 

As MMU is a part of CPU, the simulator of MMU doesn't run on a single thread. Instead it is a class `CPU_mmu` (see `src/mmu/mmu.h`). Each CPU core owns an `CPU_mmu`. When current process needs to access memory, it will finally call `CPU_mmu::read` or `CPU_MMU::write` of corresponding CPU. 

```sequence
handle -> program : handle::operator=
program -> process_t : program::write()
process_t -> CPU_mmu : process_t::write()
CPU_mmu -> Memory : mmu::write()
note over Memory : pm_write()
```

The sequence graph shows the whole procedure of memory access operation in Hyper OS.

### Page table

MMU will translate linear address of current process to physical address using page table. In Hyper OS, all the process using single-level page table, which contains $$2^{20}$$ pages. The definition of page table is in `src/mm/page_table.h`. 

Page table is setting by kernel during process switching using `CPU_core::set_context` and MMU gets it using `CPU_core::get_context`. The definition of context is in `src/context/context.h`.

If the accessed page doesn't exist, MMU will send a page fault interrupt to APIC and wait until the page is swap into memory by ISR (see `src/interrupt/interrupts/intr_pagefault.h`).

## Physical Memory Manager

Physical Memory Manager manages all page frames. It provides interface to get page frames for process loader (see `src/process/process_load.cpp` and `docs/4user/program.md 'Program Memory Layout'`) and other parts of kernel.

- `page_frame* alloc_pages(int n)`: allocate `@n` continuous page frames and return the first page frame.
- `void free_pages(page_frame *pg)`: free continuous page frames start from `@pg`. It causes undefined behaviour when `@pg` is not start of continuous pages allocated by `alloc_pages`.

PMM allocate pages using `utils/allocator`, for more information see `docs/4user/utils`.

## Example : Page Fault Handling 

This graph shows details of page fault handling. Run the example in Hyper OS : 

```bash
hyper-shell:$ demo-pf
```

```sequence
note over handle : create variable on stack 
note over handle : handle::handle<T>(const T&)
handle -> dmm of program: program::stack_push
dmm of program -> dmm of process : process_t::stack_push
dmm of process -> handle :
note over handle : write on stack
handle -> CPU_mmu : program::write -> process_t::write -> CPU_MMU::write
note over CPU_mmu : translation fail
CPU_mmu -> APIC : interrupt
APIC -> PF ISR : intr_pagefault::process
PF ISR -> pmm : swap_in
pmm -> PF ISR : swap in the page
PF ISR -> APIC : send_end_of_interrupt
APIC -> CPU_mmu : 
note over CPU_mmu : tranlation success
CPU_mmu -> memory : pm::write
note over memory : pm_write()
memory -> CPU_mmu : 
CPU_mmu -> handle : 
note over handle : write success
```

- The program wants to create an variable on stack using `handle<T>::handle<T>(const T &val)`, which calls `program::stack_push` (see `src/program/dmm.h`) and then calls `process_t::stack_push` (see `src/process/proc_dmm.h`) and allocate memory for the variable. 

- Then the program initialize the variable on stack, which causes memory write operation and calls `program::write`, `process_t::write` and `CPU_mmu::write` (see graph in section `Memory Manage Unit`). However, the accessed page doesn't exist in memory so it causes page fault. MMU sends an interrupt to APIC using `local_apic::interrupt` (see `src/apic/local_apic.h`). 

- After that, APIC starts ISR of page fault which calls `intr_pagefault::process` (see `src/interrupt/interrups/intr_pagefault.cpp`). Then PMM find a page frame, swap in current page and finish ISR by `local_apic::send_end_of_interrupt`. 

- After APIC start user process again, MMU try to translate address again using `CPU_mmu::check()` (see `src/mmu/mmu.cpp`) and succeeded. MMU then calls `pm::write`.