/*
 * Physical Memory Simulater
 */

#include "../logging/logging.h"
#include "../env/env.h"
#include "../tools/bus.h"
#include "../status/status.h"
#include "pmem_info.h"
#include <cstdint>
#include <assert.h>
#include <thread>

using std::thread;
using logging::debug;
using logging::info;
using logging::log_endl;

class pm_page_frame {
public:
	pm_page_frame()
	{
		data = nullptr;
		alloced = false;
	}
	~pm_page_frame()
	{
		if (!alloced)
			delete[] data;
	}
	char& at(size_t pt)
	{
		assert(pt < PAGE_SIZE);
		return data[pt];
	}
	bool isalloced()
	{ return alloced; }
	void alloc()
	{
		assert(!alloced);
		data = new char[PAGE_SIZE];
		alloced = true;
	}
private:
	char *data;
	bool alloced;
};

// info mmu --> pm
struct pm_info {
    enum TYPE {READ, WRITE, SHUTDOWN} ;
	TYPE type;
	size_t paddr;
	char data;
};
static bus<pm_info> mmu2pm;

// info pm --> mmu
struct pm_result {
	enum TYPE {OK, ERR};
	TYPE type;
	char data;
	size_t error_code;
};
static bus<pm_result> pm2mmu;

static pm_page_frame *pm_pages = nullptr;
// pm page array
static thread th_pm;
// pm thread

/* physical memory read, read a byte from @paddr
 * used only by memory thread
 * @paddr : physical address to read
 */
static pm_result pm_read(size_t paddr)
{
	assert((paddr/PAGE_SIZE) < PAGE_NUM);
	size_t page = paddr / PAGE_SIZE, offset = paddr % PAGE_SIZE;
	if (!pm_pages[page].isalloced())
		pm_pages[page].alloc();
	pm_result result;
	result.type = pm_result::OK;
	result.data = pm_pages[page].at(offset);
	return result;
}

/**
 * physical memory write, write a byte to @paddr
 * used only by memory thread
 * @paddr : physical address to write
 * @data  : data to write
 */
static pm_result pm_write(size_t paddr, char data)
{
	assert((paddr/PAGE_SIZE) < PAGE_NUM);
	size_t page = paddr / PAGE_SIZE, offset = paddr % PAGE_SIZE;
	if (!pm_pages[page].isalloced())
		pm_pages[page].alloc();
	pm_pages[page].at(offset) = data;
	pm_result result;
	result.type = pm_result::OK;
	return result;
}

/**
 * thread to simulate memory
 * read from bus mmu2pm
 * write from bus pm2mmu
 * --------------------------
 * Instructions Support:
 * pm_info::READ     : read a byte from pm_info::paddr
 * pm_info::WRITE    : write pm_info::data to pm_info::paddr
 * pm_info::SHUTDOWN : shutdown memory 
 */
static void memory_main()
{
	status.set_name("memory");
	logging::info << "physical memory online." << logging::log_endl;
	while (1) {
		pm_info info = mmu2pm.read();
		pm_result result;
		switch(info.type) {
	    case pm_info::READ:
			result = pm_read(info.paddr);
			break;
		case pm_info::WRITE:
			result = pm_write(info.paddr, info.data);
			break;
		case pm_info::SHUTDOWN:
			logging::info << "physical memory offline." << logging::log_endl;
			result.type = pm_result::OK;
			pm2mmu.write(result);
			return;
		default:
			assert(false);
		}
		pm2mmu.write(result);
	}
}

/* init physical memory */
void init_pm()
{
	
	logging::info << "pm init." << logging::log_endl;
	pm_pages = new pm_page_frame[PAGE_NUM];
	th_pm = thread(memory_main);
	th_pm.detach();
	logging::info << "pm online." << logging::log_endl;
}

/* showdown physical memory */
void destroy_pm()
{
	// shutdown memory thread
	pm_info info;
	info.type = pm_info::SHUTDOWN;
	mmu2pm.write(info);
	pm_result result;
	result = pm2mmu.read();
	if (result.type == pm_result::OK) {
		logging::info << "pm offline."
					  << logging::log_endl;
	} else {
		logging::info << "pm error while shutting down."
			<< "error code : "
			<< result.error_code
			<< logging::log_endl;
	}

    // clean pm page frames
    assert(pm_pages != nullptr);
	delete[] pm_pages;
	
}

/**
 * interface for mmu, read a byte
 * @paddr : physical address to read
 */
size_t read(size_t paddr)
{
	pm_info info;
	info.type = pm_info::READ;
	info.paddr = paddr;
	mmu2pm.write(info);
	pm_result result = pm2mmu.read();
	// waiting for pm
	if (result.type == pm_result::OK) {
		return result.data;
	} else {
		logging::info << "pm read fail. error code = "
					  << result.error_code << logging::log_endl;
		assert(false);
		return 0;
	}
}

/**
 * interface for mmu, write a byte
 * @paddr : physical address to write
 * @data  : data to write
 */
void write(size_t paddr, char data)
{
	pm_info info;
	info.type = pm_info::WRITE;
	info.paddr = paddr;
	info.data = data;
	mmu2pm.write(info);
	pm_result result = pm2mmu.read();
	// waiting for pm
	if (result.type == pm_result::ERR) {
		logging::info << "pm write fail. error code = "
					  << result.error_code << logging::log_endl;
	}
}

/
void debug_pm()
{
	write(10, 1);
	assert(read(10) == 1);
	write(1 << 20, 127);
	write(1 << 29 | 1, 2);
	assert(read(1 << 20) == 127);
	assert(read(1 << 29 | 1) == 2);
	write(10, 0);
	write(1 << 20, 0);
	write(1 << 29 | 1, 0);
	debug << "pm check ok." << log_endl;
}
