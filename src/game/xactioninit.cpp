#include "game/xact.h"

/**
 * @note Address: 0x800BBD7C
 * @note Size: 0x30
 */
// XAction::Init
extern "C" void Init__7XActionFP7XObjChr(XAction* action, XObjChr* owner)
{
	action->mOwner = owner;
	action->Reset();
}
