#include "game/xobjactor.h"
#include "game/xchr.h"

/**
 * @note Address: 0x80128230
 * @note Size: 0x38C
 */
// lets go of what she carries
// NONMATCHING: the first flag test
void XObjActorMama::Drop()
{
	s32 kind = -1;
	if (_C8C.Check(4)) {
		_C8C.mFlags &= ~4;
		kind = 0x7D;
	} else if (_C8C.Check(1)) {
		_C8C.mFlags &= ~1;
		kind = 0x40;
	} else if (_C8C.Check(2)) {
		_C8C.mFlags &= ~2;
		kind = 0x5B;
	} else if (_C8C.Check(8)) {
		_C8C.mFlags &= ~8;
		kind = 0x196;
	} else if (_C8C.Check(0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x1A1;
	} else if (_C8C.Check(0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x45B;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x5B:
	case 0x7D:
	case 0x196:
	case 0x45B:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (obj->GetKind() == 0x1A0) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			} else if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x8012926C
 * @note Size: 0xB0
 */
void XObjActorMama::vf15C()
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPadReset = TRUE;
	}
	_998.mFlags = 0;
	_C8C.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0;
	mState      = &XObjActorMama::StateNone;
	fn_8010706C(this);
}
