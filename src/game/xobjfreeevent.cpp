#include "game/xobjfree.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/globals.h"
#include "game/evt.h"
#include "game/xobjchr.h"

#pragma section RX ".text.freerelease"
/**
 * @note Address: 0x800DE038
 * @note Size: 0x28
 */
__declspec(section ".text.freerelease") void XObjFree::Release()
{
	mMatAnim = NULL;
	Release__4XObjFv(this);
}

// XObjFree::ResetObjectState
/**
 * @note Address: 0x800DE060
 * @note Size: 0xEC
 */
#pragma section RX ".text.freereset"
extern "C" __declspec(section ".text.freereset") void ResetObjectState__8XObjFreeFv(XObjFree* object)
{
	ResetObjectState__4XObjFv(object);
	if (object->mAttr._00 != NULL)
		object->mLife = (s16)object->GetObjFlags();
	object->mMatAnim = NULL;
	if (object->mJObj != NULL && (object->mStatus.mFlags & 0x1000ULL)) {
		UnkStageObj* animation = object->mCoord._00;
		object->SetAnim(animation->_30, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	}
}
