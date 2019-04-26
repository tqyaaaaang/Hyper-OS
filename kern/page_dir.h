/* Data Structures for Page Dir 
 */
#pragma once
#include <cstdint>
#include "defs.h"

class page_dir {

public:
	
	size_t entry[vm_size / page_size]; 
};
