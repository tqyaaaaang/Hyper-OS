/**
 * syscall/syscall.h
 * System call entry
 */

#pragma once

class syscall_t;

int syscall ( syscall_t *current_syscall );
