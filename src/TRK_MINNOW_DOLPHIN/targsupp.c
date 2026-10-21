#include "PowerPC_EABI_Support/MetroTRK/trk.h"

// originally an assembled file, these just trap into the debugger which does the actual file work
// the linker can't strip pieces of an assembled object, open and position are never called so keep them all
#pragma push
#pragma force_active on

// the original file starts 16 aligned, only japan has a gap before it where that shows
#pragma function_align 16

/**
 * @note Address: 0x8014F1A0
 * @note Size: 0x8
 */
ASM u32 TRKAccessFile(u32 command, u32 handle, u32* count, u8* buffer) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	twi 31, r0, 0
	blr
#endif // clang-format on
}

#pragma function_align 4

/**
 * @note Address: 0x8014F1A8
 * @note Size: 0x8
 */
ASM u32 TRKOpenFile(u32 command, u32 handle, u32* count, u8* buffer) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	twi 31, r0, 0
	blr
#endif // clang-format on
}

/**
 * @note Address: 0x8014F1B0
 * @note Size: 0x8
 */
ASM u32 TRKCloseFile(u32 command, u32 handle) {
#ifdef __MWERKS__ // clang-format off
	nofralloc
	twi 31, r0, 0
	blr
#endif // clang-format on
}

/**
 * @note Address: 0x8014F1B8
 * @note Size: 0x8
 */
ASM u32 TRKPositionFile(u32 command, u32 handle, u32* count, u8* buffer)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	twi 31, r0, 0
	blr
#endif // clang-format on
}

#pragma pop
