#include "types.h"

struct PointPtclOne {
	s32 mState;   // _00
	u8 _04[0x20]; // _04
};
STATIC_ASSERT(sizeof(PointPtclOne) == 0x24);

// the sparks that fly in when points change, a pool of 10 at lbl_80378300. type 0, 2 and 3
// have their own colors, a negative amount draws them black
struct PointPtcl {
	u8 _00[0x8];             // _00
	s32 mType;               // _08
	s32 mState;              // _0C
	s32 mAmount;             // _10
	u8 _14[0x54];            // _14
	u32 mColor;              // _68
	u32 mColor2;             // _6C
	u8 _70[0x4];             // _70
	s32 _74;                 // _74
	s32 mNum;                // _78
	PointPtclOne mPtcls[40]; // _7C
};
STATIC_ASSERT(sizeof(PointPtcl) == 0x61C);

// todo: cleanup
static inline s32 PtclNum(s32 amount)
{
	s32 sign = amount >> 31;
	return (s32)((u32)(amount ^ sign) - (u32)sign);
}

/**
 * @note Address: 0x80095AD8
 * @note Size: 0x1F8
 */
extern "C" void fn_80095AD8(PointPtcl* ptcl, s32 type, s32 amount)
{
	ptcl->mType   = type;
	ptcl->mState  = 1;
	ptcl->mAmount = amount;
	switch (ptcl->mType) {
	case 0:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xE037EE00 : 0xFF000000;
		ptcl->mNum    = PtclNum(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -1;
		break;
	case 2:
		ptcl->mColor  = amount >= 0 ? 0xEFEBC500 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xEDA83600 : 0xFF000000;
		ptcl->mNum    = PtclNum(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -2;
		break;
	case 3:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0x5959FF00 : 0xFF000000;
		ptcl->mNum    = PtclNum(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -3;
		break;
	default:
		ptcl->mColor  = 0xFFFFFF00;
		ptcl->mColor2 = 0xFFFFFF00;
		ptcl->mNum    = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = 5;
		break;
	}
}
