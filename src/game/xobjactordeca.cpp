#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"

// XObjActorDeca

void XObjActorDeca::vf150()
{
	mState = NULL;
}

void XObjActorDeca::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

static inline MtxPtr DecaMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}
