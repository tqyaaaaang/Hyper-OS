#include "test_pmm.h"
#include "../src/mm/pmm.h"
#include "../src/mm/pmem.h"
#include "../src/logging/logging.h"
#include <cassert>

using logging::debug;
using logging::log_endl;

void test_pmm()
{
	page_frame* pg[8];
	for (int i = 0; i < 8; i++) { 
		pg[i] = alloc_pages((i + 1) * 10);
		pg[i]->ref();
	}
	for (int i = 0; i < 8; i++)
		for (int j = i + 1; j < 8; j++) {
			assert(page2id(pg[i]) + pg[i]->length <= page2id(pg[j])
				   || page2id(pg[j]) + pg[j]->length <= page2id(pg[i]));
		}
	for (int i = 0; i < 8; i++) 
		free_pages(pg[i]);
	debug << "pmm check ok." << log_endl;
}
