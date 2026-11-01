#include "game/xchr.h"

extern "C" Vec fn_800CE9A8(XAction* action, XObj* object);

/**
 * @note Address: 0x800CE904
 * @note Size: 0xA4
 */
extern "C" void fn_800CE904(XAction* action, XObj* object)
{
	object->SetPos(fn_800A0978(action->mOwner) - fn_800CE9A8(action, object), 0);
}
