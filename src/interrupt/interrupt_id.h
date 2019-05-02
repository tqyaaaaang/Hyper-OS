/**
 * interrupt/interrupt_id.h
 * Interrupt IDs
 */

#pragma once

#include <string>

enum class interrupt_id_t
{
	DIVIDE = 0,
	DEBUG,
	NMI,
	BREAKPOINT,
	OFLOW,
	BOUND,
	ILLEGAL_OPCODE,
	DEVICE,
	DOUBLE_FALUT,
	COPROC,
	TSS,
	SEGNP,
	STACK,
	GPFLT,
	PGFLT,
	RESERVED_15,
	FPERR,
	ALIGN,
	MCHK,
	SIMDERR,

	DOUBLE_INTERNAL_TEST = 30,
	INTERNAL_TEST = 31,

	END_OF_INTERRUPT = -1,
	DISABLE_LAPIC = -2,

	IO_APIC_END_OF_INTERRUPT = -100,
	DISABLE_IO_APIC = -101
};
