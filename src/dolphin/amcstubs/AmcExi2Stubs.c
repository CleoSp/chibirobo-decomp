#include "types.h"
#include "Dolphin/AmcExi2Stubs.h"

/**
 * @note Address: 0x8018D404
 * @note Size: 0x4
 */
void EXI2_Init(vu8** inputPendingPtrRef, AmcEXICallback monitorCallback)
{
	return;
}

/**
 * @note Address: 0x8018D408
 * @note Size: 0x4
 */
void EXI2_EnableInterrupts()
{
	return;
}

/**
 * @note Address: 0x8018D40C
 * @note Size: 0x8
 */
int EXI2_Poll()
{
	return 0;
}

/**
 * @note Address: 0x8018D414
 * @note Size: 0x8
 */
AmcExiError EXI2_ReadN(void* bytes, u32 length)
{
	return AMC_EXI_NO_ERROR;
}

/**
 * @note Address: 0x8018D41C
 * @note Size: 0x8
 */
AmcExiError EXI2_WriteN(const void* bytes, u32 length)
{
	return AMC_EXI_NO_ERROR;
}

/**
 * @note Address: 0x8018D424
 * @note Size: 0x4
 */
void EXI2_Reserve()
{
	return;
}

/**
 * @note Address: 0x8018D428
 * @note Size: 0x4
 */
void EXI2_Unreserve()
{
	return;
}

/**
 * @note Address: 0x8018D42C
 * @note Size: 0x8
 */
BOOL AMC_IsStub()
{
	return TRUE;
}
