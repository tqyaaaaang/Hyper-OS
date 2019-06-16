/* physical memory */
#pragma once

namespace pm {

// interface for simulater
char read(size_t paddr);
void write(size_t paddr, char data);

}

// interface for os
void init_pm();
void destroy_pm();
