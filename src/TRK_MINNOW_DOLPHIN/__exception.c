#include "types.h"

// metrowerks wrote this one in plain asm, mwcc inline asm has no data directives so the
// banner and the gaps between vectors are spelled out with opword

extern void TRKInterruptHandler(void);
extern void __TRK_reset(void);

#ifdef __MWERKS__ // clang-format off
#define PAD1  opword 0
#define PAD4  PAD1; PAD1; PAD1; PAD1
#define PAD16 PAD4; PAD4; PAD4; PAD4
#define PAD64 PAD16; PAD16; PAD16; PAD16

// hand the exception to TRKInterruptHandler with the vector in r3, r2 to r4 get stashed in sprg1 to 3
#define TRK_REDIRECT(vector) \
	mtspr SPRG1, r2; \
	mtspr SPRG2, r3; \
	mtspr SPRG3, r4; \
	mfspr r2, SRR0; \
	mfspr r4, SRR1; \
	mfmsr r3; \
	ori r3, r3, 0x30; \
	mtspr SRR1, r3; \
	lis r3, TRKInterruptHandler@h; \
	ori r3, r3, TRKInterruptHandler@l; \
	mtspr SRR0, r3; \
	li r3, vector; \
	rfi

// the 603e tlb miss ones run with msr tgpr set, so flip that off first to get the real r2 back
#define TRK_TLB_REDIRECT(vector) \
	mtspr SPRG1, r2; \
	mfcr r2; \
	mtspr SPRG2, r2; \
	mfmsr r2; \
	andis. r2, r2, 2; \
	beq restore_cr_##vector; \
	mfmsr r2; \
	xoris r2, r2, 2; \
	sync; \
	mtmsr r2; \
	sync; \
	mtspr SPRG1, r2; \
	restore_cr_##vector: ;; \
	mfspr r2, SPRG2; \
	mtcrf 0xFF, r2; \
	mfspr r2, SPRG1; \
	TRK_REDIRECT(vector)
#endif // clang-format on

/**
 * @note Address: 0x80003154
 * @note Size: 0x1F34
 */
DECL_SECT(".init") ASM void gTRKInterruptVectorTable(void)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc

	// "Metrowerks Target Resident Kernel for PowerPC"
	opword 0x4D657472; opword 0x6F776572; opword 0x6B732054; opword 0x61726765
	opword 0x74205265; opword 0x73696465; opword 0x6E74204B; opword 0x65726E65
	opword 0x6C20666F; opword 0x7220506F; opword 0x77657250; opword 0x43000000
	PAD16; PAD16; PAD16; PAD4

	// 0x0100 system reset
	b __TRK_reset
	PAD16; PAD16; PAD16; PAD4; PAD4; PAD4; PAD1; PAD1; PAD1

	// 0x0200 machine check, flush the line it died on first
	mtspr SPRG1, r2
	mfspr r2, SRR0
	icbi 0, r2
	mfdar r2
	dcbi 0, r2
	mfspr r2, SPRG1
	TRK_REDIRECT(0x200)
	PAD16; PAD16; PAD4; PAD4; PAD4; PAD1

	// 0x0300 dsi
	TRK_REDIRECT(0x300)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0400 isi
	TRK_REDIRECT(0x400)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0500 external interrupt
	TRK_REDIRECT(0x500)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0600 alignment
	TRK_REDIRECT(0x600)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0700 program
	TRK_REDIRECT(0x700)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0800 fpu unavailable
	TRK_REDIRECT(0x800)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0900 decrementer
	TRK_REDIRECT(0x900)
	PAD64; PAD64; PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0C00 system call
	TRK_REDIRECT(0xC00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0D00 trace
	TRK_REDIRECT(0xD00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0E00 fp assist
	TRK_REDIRECT(0xE00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x0F00 performance monitor, which has to jump over 0x0F20 altivec unavailable
	b perf_monitor
	PAD4; PAD1; PAD1; PAD1
	TRK_REDIRECT(0xF20)
perf_monitor:
	TRK_REDIRECT(0xF00)
	PAD16; PAD4; PAD4; PAD4; PAD1; PAD1

	// 0x1000 603e instruction tlb miss
	TRK_TLB_REDIRECT(0x1000)
	PAD16; PAD16; PAD4

	// 0x1100 603e data load tlb miss
	TRK_TLB_REDIRECT(0x1100)
	PAD16; PAD16; PAD4

	// 0x1200 603e data store tlb miss
	TRK_TLB_REDIRECT(0x1200)
	PAD16; PAD16; PAD4

	// 0x1300 instruction address breakpoint
	TRK_REDIRECT(0x1300)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1400 system management interrupt
	TRK_REDIRECT(0x1400)
	PAD64; PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1600 denorm detect
	TRK_REDIRECT(0x1600)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1700 thermal management interrupt
	TRK_REDIRECT(0x1700)
	PAD64; PAD64; PAD64; PAD64; PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1C00 data breakpoint
	TRK_REDIRECT(0x1C00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1D00 instruction breakpoint
	TRK_REDIRECT(0x1D00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1E00 peripheral breakpoint
	TRK_REDIRECT(0x1E00)
	PAD16; PAD16; PAD16; PAD1; PAD1; PAD1

	// 0x1F00 non maskable development port
	TRK_REDIRECT(0x1F00)
#endif // clang-format on
}
