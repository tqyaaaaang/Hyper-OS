/**
 * utils/pagerepl/pagerepl.h
 * page replacement algorithm's interface 
 */
#pragma once
#include <cstddef>
#include <cstdint>

class page_repl {
public:

	page_repl();
	virtual ~page_repl();

	/**
	 * swap in page @id 
	 */
	virtual void page_swap_in(size_t id) = 0;

	/**
	 * swap out a page
	 * @return : the page to swap out 
	 */
	virtual size_t page_swap_out() = 0;

	bool full();
	void swap_in(size_t id);
	size_t swap_out();

	size_t get_page_num() const;
	void set_page_num(size_t page_num);

private:
	size_t cur_num;
    size_t page_num;
};
