/**
 * utils/assert.h
 */

#pragma once

#define kern_assert(x) if (!x) {				\
	printf("assertion fail : %s", #(x));		\
	exit(1);
