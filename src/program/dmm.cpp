#include "program.h"
#include "../utils/allocator/allocator.h"
#include "../utils/allocator/ffma.h"
#include "../process/process_t.h"

/* temp */
size_t program::heap_malloc(size_t len)
{
	return cur_proc->heap_malloc(len);
}

/* temp */
void program::heap_free(size_t ptr)
{
	cur_proc->heap_free(ptr);
}

size_t program::stack_push(size_t size)
{
	return cur_proc->stack_push(size);
}

size_t program::stack_pop(size_t size)
{
	if (cur_proc->get_exit_flag()) {
		return 0;
	}
	return cur_proc->stack_pop(size);
}

