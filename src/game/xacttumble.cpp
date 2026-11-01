#include "game/xact.h"

/**
 * @note Address: 0x800C490C
 * @note Size: 0x40
 */
extern "C" void fn_800C490C(XActTumble* act)
{
	act->Reset();
	act->_44 = 0;
}
