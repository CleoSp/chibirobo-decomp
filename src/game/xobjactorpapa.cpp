#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"
#include <stdlib.h>

static inline MtxPtr ActorMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

HSD_JObj* XObjActorPapa::vf168()
{
	s32 path[3] = { 0, 2, 0 };
	return FindJoint(mJObj, 3, path);
}

CVec XObjActorPapa::vf16C()
{
	s32 path[3] = { 0, 2, 0 };
	return fn_8007E7C8(this, 3, path);
}

bool XObjActorPapa::IsFree()
{
	if ((mFlags & 1) == 0) {
		return true;
	}
	return !(mStatus.mFlags & 0x2000);
}

f32 XObjActorPapa::vf17C()
{
	return -0.17453292f;
}

f32 XObjActorPapa::vf178()
{
	return 0.17453292f;
}
