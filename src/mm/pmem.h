/* physical memory */
#pragma once

// interface for simulater
size_t read(size_t paddr);
void write(size_t paddr, char data);

// interface for os
void init_pm();
void destroy_pm();
