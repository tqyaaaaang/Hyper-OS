#pragma once
#include <cstdint>
#include <cstddef>
#include <mutex>

extern void alltraps();

extern std::mutex cpu_owner;

class intr_info {

public:
	size_t intr_id;
};

class page_fault_info : public intr_info {
	
public:
	size_t la;         // linear address
	bool option;       // read 1 / write 0	
};

void intr_init();          // init interrupt admin
void intr_on();            // interrupt on
void intr_off();           // interrupt off
bool intr_enabled();       // test if interrupt enabled
void intr_alert(intr_info*); // int
