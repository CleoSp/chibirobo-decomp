#include "types.h"
#include "Dolphin/OS/OSInterrupt.h"

/**
 * @note Address: 0x801503A8
 * @note Size: 0x8
 */
WEAK int udp_cc_initialize(void* flagOut, __OSInterruptHandler handler)
{
	return -1;
}

/**
 * @note Address: 0x801503A0
 * @note Size: 0x8
 */
WEAK int udp_cc_shutdown(void)
{
	return -1;
}

/**
 * @note Address: 0x80150398
 * @note Size: 0x8
 */
WEAK int udp_cc_open(void)
{
	return -1;
}

/**
 * @note Address: 0x80150390
 * @note Size: 0x8
 */
WEAK int udp_cc_close(void)
{
	return -1;
}

/**
 * @note Address: 0x80150388
 * @note Size: 0x8
 */
WEAK int udp_cc_read(u8* dest, int size)
{
	return 0;
}

/**
 * @note Address: 0x80150380
 * @note Size: 0x8
 */
WEAK int udp_cc_write(const u8* src, int size)
{
	return 0;
}

/**
 * @note Address: 0x80150378
 * @note Size: 0x8
 */
WEAK int udp_cc_peek(void)
{
	return 0;
}

/**
 * @note Address: 0x80150370
 * @note Size: 0x8
 */
WEAK int udp_cc_pre_continue(void)
{
	return -1;
}

/**
 * @note Address: 0x80150368
 * @note Size: 0x8
 */
WEAK int udp_cc_post_stop(void)
{
	return -1;
}
