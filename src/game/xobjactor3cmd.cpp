#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include "game/evt.h"
#include "game/video.h"

extern "C" {
f32 fn_8006697C(f32 frames);
}

static inline void ClearCommand(XObjActor3* actor)
{
	actor->_B94 = 0;
	actor->_B98 = 0;
}

// anims chibi robo can't be interrupted in
static inline bool BlockingAnimation(s32 animation)
{
	switch (animation) {
	case 0x5:
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0x27:
	case 0x28:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x43:
	case 0x97:
	case 0xA1:
	case 0xED:
	case 0xEE:
	case 0xF6:
	case 0x814:
	case 0x816:
	case 0x817:
	case 0x81F:
	case 0x820:
	case 0x824:
	case 0x879:
	case 0x87A:
	case 0x8AE:
	case 0x8AF:
	case 0x8B7:
	case 0x8C0:
	case 0x8C1:
	case 0x8C2:
	case 0x8C3:
	case 0x8C4:
	case 0x8C5:
		return true;
	default:
		return false;
	}
}

/**
 * @note Address: 0x8010B394
 * @note Size: 0x194
 */
extern "C" void fn_8010B394(XObjActor3* actor)
{
	switch (actor->_B94) {
	case 1:
		if (actor->_B98) {
			actor->_BEC = 3;
		} else {
			actor->_BEC = 4;
		}
		ClearCommand(actor);
		break;
	case 2:
		if (actor->_B98) {
			actor->_BF8 = 1;
		} else {
			actor->_BF8 = 0;
		}
		ClearCommand(actor);
		break;
	case 3:
#if defined(VERSION_PAL)
		actor->_BF4 = (f32)lbl_80660D4C * ((f32)actor->_B98 / 100.0f);
#else
		actor->_BF4 = 60.0f * ((f32)actor->_B98 / 100.0f);
#endif
		ClearCommand(actor);
		break;
	case 4:
		lbl_80367A2C[actor->_B98] = (s32)fn_8006697C(actor->_BF4);
		ClearCommand(actor);
		break;
	case 5:
		if (actor->_B98) {
			actor->_BFC = 3;
		} else {
			actor->_BFC = 4;
		}
		ClearCommand(actor);
		break;
	case 6:
		actor->_C04 = actor->_B98;
		ClearCommand(actor);
		break;
	case 7:
		lbl_80367A2C[actor->_B98] = actor->_C04;
		ClearCommand(actor);
		break;
	}
}

/**
 * @note Address: 0x8010B528
 * @note Size: 0x28C
 */
extern "C" void fn_8010B528(XObjActor3* actor)
{
	switch (actor->_B94) {
	case 1:
		if (actor->_B98) {
			actor->_B8C.mFlags &= ~4U;
			actor->_BC0 = (f32)actor->_B9C / 100.0f;
			actor->_BC4 = (f32)actor->_BA0 / 100.0f;
		} else {
			actor->_B8C.mFlags |= 4;
			actor->_BC0 = 0.0f;
			actor->_BC4 = 0.0f;
			actor->_B80 = 0;
		}
		ClearCommand(actor);
		break;
	case 2:
		if ((actor->_B8C.mFlags & 0x80) && !(actor->_B8C.mFlags & 4)) {
			lbl_80367A2C[actor->_B98] = 1;
		}
		ClearCommand(actor);
		break;
	case 3:
		actor->_B8C.mFlags &= ~0x80U;
		ClearCommand(actor);
		break;
	case 4: {
		XObjChr* player = lbl_8025C400.mPlayer;
		if (!BlockingAnimation(player->mAnimId) && player->mAttachmentRequest < 0) {
			lbl_80367A2C[actor->_B98] = 1;
		}
		ClearCommand(actor);
		break;
	}
	}
}
