#include "debug.h"
#include "../mm/pmm.h"
#include "../tools/allocator/ffma.h"

void debug()
{
	debug_pmm();
	debug_ffma();
}
