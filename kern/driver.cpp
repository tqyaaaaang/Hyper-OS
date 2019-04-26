#include "intr.h"
#include "state_info.h"
#include "defs.h"
#include "vclock.h"

int main()
{
	state_init();
	intr_init();
	clock_init();
	return 0;
}
