#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactorfamily.h"

static inline Vec TaoPosition(const CJObj& obj)
{
	return GetJObjPos(obj.mJObj);
}

static inline MtxPtr TaoMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

void XObjActorTao::vf158(u32 frames)
{
	if (mStatus.Check(2)) {
		mStatus.mFlags &= ~2;
		fn_8001E904(GAME_SOUND, 0x15D, 0xFF);
	}
}

void XObjActorTao::vf15C()
{
	mStatus.mFlags = 0;
	_C8C.mFlags    = 0;
	_980           = 0.0f;
	_984           = 0.0f;
	_988           = 0.0f;
	_C84           = _988;
	_C88           = 0;
	mState         = &XObjActorTao::StateNone;
	fn_8010706C(this);
	_9E8[0] = 0;
	_9E8[1] = 0;
	_9E8[2] = 0;
	_9E8[3] = 0;
	_9E8[4] = 0;
	_9E8[5] = 0;
	_9E8[6] = 0;
	_9E8[7] = 0;
	_CF4    = -1;
	_CF8    = 0;
	_CFC    = NULL;
	_CF4    = 0x82;
	_CF8    = 1;
	mStatus.mFlags |= 0x2000;
}
