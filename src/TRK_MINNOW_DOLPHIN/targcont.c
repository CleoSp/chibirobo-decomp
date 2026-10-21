#include "PowerPC_EABI_Support/MetroTRK/trk.h"

/**
 * @note Address: 0x8015011C
 * @note Size: 0x34
 */
DSError TRKTargetContinue(void)
{
	TRKTargetSetStopped(0);
	UnreserveEXI2Port();
	TRKSwapAndGo();
	ReserveEXI2Port();
	return DS_NoError;
}
