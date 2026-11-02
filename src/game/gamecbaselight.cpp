#include "game/cgame.h"

/**
 * @note Address: 0x80019310
 * @note Size: 0x2C
 */
extern "C" CLight* GetLight__5CBaseFv(CBase* base)
{
	return base->vf3C();
}
