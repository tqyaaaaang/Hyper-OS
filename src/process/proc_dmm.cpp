#include "process_t.h"
#include "../utils/allocator/ffma.h"
#include <cassert>
#include "../logging/logging.h"

using logging::info;
using logging::debug;
using logging::log_endl;

/**
 * init dynamic memory manager
 */
void process_t::init_dmm()
{
	info << "init dmm." << log_endl;
	heap_allocator = new
		ffma(VM_SIZE - prog->get_data_size()
			 - prog->get_bss_size());
	info << "finished." << log_endl;
}

/**
 * malloc continuous memory of @len bytes
 * using ffma algorithm
 * @len : length of continuous memory
 * @return : address of the start of the memory 
 */
size_t process_t::heap_malloc(size_t len)
{
	size_t offset = prog->get_data_size()
		+ prog->get_bss_size() + sizeof(size_t);
	size_t ptr = offset + heap_allocator->
		malloc(len + sizeof(size_t));
	vm_write(ptr - sizeof(size_t),
			 (const char*)&len,
			 (const char*)((&len) + 1));
	return ptr;
}

/**
 * free continuous memory start from @ptr
 * @ptr : start of the memory
 */
void process_t::heap_free(size_t ptr)
{
	size_t len = -1;
	char *pt = (char*)&len;
	vm_read(pt,
			ptr - sizeof(size_t),
			ptr);
	assert(len != -1);
	size_t offset = prog->get_data_size()
		+ prog->get_bss_size();
	ptr -= offset + sizeof(size_t);
    heap_allocator->free(ptr, len + sizeof(size_t));
}

/**
 * push stack 
 * @size : the size of object to push
 * @return : the address of new stack top
 */
size_t process_t::stack_push(size_t size)
{
	info << "push : " << size << log_endl;
	context.esp -= size;
	return context.esp;
}

/**
 * pop stack
 * @size : the size of object to pop
 * @return : the address of new stack top
 */
size_t process_t::stack_pop(size_t size)
{
	info << "pop : " << size << log_endl;
	context.esp += size;
	return context.esp;
}
