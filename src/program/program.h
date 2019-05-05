/**
 * program/program.h
 * abstract base class for all user-mode programs
 */
#pragma once
#include <cstddef>
#include <cstdint>

class program;

/**
 * handle of type T
 */
template<typename T>
class handle {
public:
	handle();
	handle(program *prog);
	handle(size_t addr, program *prog);
	handle& operator = (const handle &val);
	handle& operator = (const T &val);
	operator T() const;
	size_t get_addr() const;
	program* get_prog() const;
	  
private:
    size_t addr;       // start address of T 
    program *prog;     // program containing this handle
};

/**
 * base type of program
 * - memory layout of a program
 * ---------------- 0x00000000
 * |    .text     |
 * |--------------| text_size
 * |    .data     |
 * |--------------| text_size + data_size
 * |     .bss     |
 * |--------------| text_size + data_size + bss_size
 * |    .stack    |
 * |   heap(up)   |  
 * |              |
 * |  stack(down) |
 * |--------------| VM_SIZE
 */
class program {
public:

    program();
	~program();

	// data/text/bss size is set during compiling ( static_init )
	size_t get_text_size() const;          // size of text(code) segment
	size_t get_data_size() const;          // size of data segment
	size_t get_bss_size() const;           // size of bss segment
	size_t get_stack_size() const;         // size of stack/heap segment

	void run();                            // set running
	bool is_running() const;               // check if the process is running
	
	virtual void static_init();            // init static info & data
    virtual void main() = 0;               // program entry
	void compile();                        // simulate compile

	template<typename T>
	handle<T> alloc_static();
	// alloc memory in data segment

	template<typename T>
	handle<T> alloc_bss();
	// alloc memory in bss segment
	
	template<typename T>
    handle<T> alloc_stack();
	// alloc memory in stack

	template<typename T>
    handle<T> alloc_heap();
	// alloc memory in heap

	// temp
	char prog_read(size_t addr);
	void prog_write(size_t addr, char data);
	
private:
	size_t text_size;
	size_t data_size;
	char* data;
	size_t bss_size;
	size_t stack_size;
	bool running;

    // heap operation
	size_t heap_malloc(size_t size);
	void heap_free(size_t addr);

	// stack operation
	void stack_push(size_t data);
	void stack_pop();

};
