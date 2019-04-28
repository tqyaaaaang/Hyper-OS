/* physical memory */
#pragma once

// interface for simulater
size_t read(size_t paddr);
void write(size_t paddr, char data);

void pm_init();
void pm_shutdown();
