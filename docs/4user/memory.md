# Memory and Memory Manager

## Physical Memory

Hyper OS considers physical memory as a special hardware. Like other hardware, physical memory runs on a **single thread** (with name `memory` in `status.name`), waiting for memory access instructions of MMU. Main function of memory thread is in `src/mm/pmem.cpp, memory_main()`.

MMU use the interface `pm::read` and `pm::write`, which will send a signal (`pm_info`) to physical memory thread and waiting for result (`pm_result`). 

- `size_t read(size_t paddr)`: read a byte from physical address `@paddr`, and return the result.
- `void write(size_t paddr, char data)`: write a byte `@data` to physical address `@paddr`.

After receiving signal from `MMU`, physical memory simulator calls functions `pm_read` and `pm_write`.

In order to reduce demand of memory, physical memory of Hyper OS allocate memory dynamically. Physical memory is divided into page frames in size of 4KB, only visited pages is actually allocated.

## Memory Manage Unit

Memory manage unit (MMU) is the key to paging. It provides interface of virtual memory read/write, translating the address to physical address using page table and access physical memory. 

As MMU is a part of CPU, the simulator of MMU doesn't run on a single thread. Instead it is a class `CPU_mmu` (see `src/mmu/mmu.h`). Each CPU core owns an `CPU_mmu`. When current process needs to access memory, it will call `CPU_mmu::read` or `CPU_MMU::write` of corresponding CPU. 



## Physical Memory Manager

## Utilities : Memory Allocator

