#include "pmm.h"
#include "intr.h"
#include "state_info.h"
#include "defs.h"
#include "vclock.h"

int main()
{
	state_init();
	intr_init();
	pm_init();
	clock_init();
	// mmu_init();
	// vm_init();
	// process_init(); 
	// shell_run();
	return 0;
}
