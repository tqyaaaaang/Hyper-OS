/**
 * context/context.h
 * running context of cpu
 */
#include "../mm/page_table.h"

class context_t {
public:
		
	void set_page_table( page_table *pg );
	page_table * get_page_table() const;

private:
    page_table *pg;
};
