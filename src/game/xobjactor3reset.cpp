#include "game/xobjactor.h"

extern "C" {
// the tu's vec literal, its rodata isn't split out with this unit
extern Vec lbl_8020B5C0;
extern AnimSfxEvent lbl_8022C178[]; // haisui_nezumi's anim sound events
void fn_8010AD88(XObjActor3* actor);
}

/**
 * @note Address: 0x8010AD00
 * @note Size: 0x20
 */
extern "C" void fn_8010AD00(XObjActor3* actor)
{
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010AD20
 * @note Size: 0x34
 */
extern "C" void fn_8010AD20(XObjActor3* actor)
{
	fn_800928E8(actor);
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010AD54
 * @note Size: 0x34
 */
extern "C" void fn_8010AD54(XObjActor3* actor, u32 id)
{
	fn_800929C4(actor, id);
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010AD88
 * @note Size: 0x1EC
 */
extern "C" void fn_8010AD88(XObjActor3* actor)
{
	actor->_B70        = 0.0f;
	actor->_B74        = 0.0f;
	actor->_B78        = 0.0f;
	actor->_B7C        = 0.0f;
	actor->_B80        = 0;
	actor->_B88        = 0;
	actor->_B8C.mFlags = 0;
	actor->_B8C.mFlags |= 4;
	actor->_B94 = 0;
	actor->_B98 = 0;
	actor->_B9C = 0;
	actor->_BA0 = 0;
	for (u32 i = 0; i < 7; ++i) {
		actor->_BA4[i] = 0;
	}
	actor->_BC0 = 0.0f;
	actor->_BC4 = 0.0f;
	actor->_BC8 = 0.0f;
	Vec v       = lbl_8020B5C0;
	actor->_BCC = v.x;
	actor->_BD0 = v.y;
	actor->_BD4 = v.z;
	for (u32 i = 0; i < 8; ++i) {
		actor->mParts[i]._04 = 0;
		actor->mParts[i]._00 = 0;
	}
	if (actor->_258 == 0) {
		lbl_8065B2F0 = 0;
		lbl_8065B2F4 = 0;
	}
	actor->vf12C(1, 0, 0, 0, 0.0f);
	actor->_BDC.x = 0.0f;
	actor->_BDC.y = 0.0f;
	actor->_BDC.z = 0.0f;
	actor->_BEC   = 0;
	actor->_BF0   = 0.0f;
	actor->_BF4   = 0.0f;
	actor->_BF8   = 0;
	actor->_BFC   = 0;
	actor->_C00   = 0.0f;
	actor->_C04   = 0;
	actor->_C08   = 0;
	if (actor->GetKind() == 0x31B) {
		fn_80024FAC(&actor->mPtcl, actor->mPtcls, sizeof(CPtclDust3), 50);
		actor->mAnimSfx.mEvents = lbl_8022C178;
	}
	actor->_1DC0[0] = 0;
	actor->_1DC0[1] = 0;
	actor->_1DC0[2] = 0;
	actor->_1DC0[3] = 0;
	actor->_1DC0[4] = 0;
	actor->_1DC0[5] = 0;
	actor->_1DC0[6] = 0;
	actor->_1DC0[7] = 0;
	actor->_1DC0[8] = 0;
	actor->_1DC0[9] = 0;
	actor->mSe      = -1;
}
