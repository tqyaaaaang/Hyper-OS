/**
 * interrupt/trap.h
 * Interrupt Service Routine
 */

#pragma once

#include "../status/status.h"

class interrupt_t;

void interrupt_trap_entry ( status_t thread_status, interrupt_t *current_interrupt );
