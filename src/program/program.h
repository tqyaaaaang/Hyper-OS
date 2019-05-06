/**
 * program/program.h
 * abstract base class for all user-mode programs
 */
#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

class program;

namespace handle_type {
	
	enum type {
		STATIC,
		BSS,
		STACK,
		HEAP
	};

}

/**
 * handle of type T
 */
template<typename T>
class handle {
public:

	handle();                               // construct
	handle(program *prog);                  // init with prog
	handle(size_t addr, program *prog,
		   handle_type::type t);            // init with address, prog and type
	handle& operator = (const handle &val); // copy (T)val to this->addr
	handle& operator = (const T &val);      // handle assign
	handle operator [] (int n);             // handle array access
	operator T() const;                     // auto unzip

	size_t get_addr() const;
	size_t* get_addr_addr();                // for redirect
	program* get_prog() const;

	void alias(const handle &val);          // let this handle be an alias of val
	
	handle_type::type get_type() const;     // get type
	void set_type(handle_type::type t);     // set type
	
private:
    size_t addr;       // start address of T 
    program *prog;     // program containing this handle
	handle_type::type this_type;
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
	bool is_compiling() const;             // check if the program is compiling
	
	virtual void static_init();            // init static info & data
    virtual void main() = 0;               // program entry
	void compile();                        // simulate compile

	void add_redirect_bss(size_t *addr);   // add addr into bss redirect table
	
	template<typename T>
	handle<T> alloc_static();
	// alloc memory in data segment

	template<typename T>
	handle<T> alloc_static(size_t n);
	// alloc continious memory for T * n on data
	// return handle to first T

	template<typename T>
	handle<T> alloc_bss();
	// alloc memory in bss segment

	template<typename T>
    handle<T> alloc_bss(size_t n);
	// alloc continious memory for T * n on bss
	// return handle to first T
	
	template<typename T>
    handle<T> alloc_stack();
	// alloc memory in stack
	
	template<typename T>
    handle<T> alloc_heap();
	// alloc memory in heap

	template<typename T>
	handle<T> alloc_heap(size_t n);
	// alloc continious memory for T * n on heap
	// return handle of first T 

	template<typename T>
	void free_heap(handle<T> ptr);
	// free continious memory start from ptr 
	
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
	bool compiling;
	
	size_t alloc_static_area(size_t len);
	size_t alloc_bss_area(size_t len);
	
    // heap operation
	size_t heap_malloc(size_t size);
	void heap_free(size_t addr);

	// stack operation
	void stack_push(size_t data);
	void stack_pop();

	std::vector<size_t*> redr_table;
};
