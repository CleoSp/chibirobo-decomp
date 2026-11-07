#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cscrn.h"
extern "C" {
#include <sysdolphin/random.h>
}

static inline u32 MaskedFlags(const CStatus& status, u32 mask)
{
	return status.mFlags & mask;
}

/**
 * @note Address: 0x80128230
 * @note Size: 0x38C
 */
// lets go of what she carries
void XObjActorMama::Drop()
{
	s32 kind = -1;
	if (MaskedFlags(_C8C, 4)) {
		_C8C.mFlags &= ~4;
		kind = 0x7D;
	} else if (MaskedFlags(_C8C, 1)) {
		_C8C.mFlags &= ~1;
		kind = 0x40;
	} else if (MaskedFlags(_C8C, 2)) {
		_C8C.mFlags &= ~2;
		kind = 0x5B;
	} else if (MaskedFlags(_C8C, 8)) {
		_C8C.mFlags &= ~8;
		kind = 0x196;
	} else if (MaskedFlags(_C8C, 0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x1A1;
	} else if (MaskedFlags(_C8C, 0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x45B;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = gGame.mXObjs.mPools[8].Get(i);
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
			XObj* obj = gGame.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = gGame.mXObjs.mPools[22].Get(i);
			if (obj->GetKind() == 0x1A0) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			} else if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

void XObjActorMama::vf158()
{
	CJObj jobj;
	XObjChr* player = gGame.GetPlayer();
	CVec pos;
	if (_998.Check(0x400)) {
		_998.mFlags &= ~0x400;
		mHeadPitch = 0.0f;
	}
	if (_998.Check(2)) {
		_998.mFlags &= ~2;
		fn_8001E904(&gGame.mSound, HSD_Randi(10) + 0x9E, 0xFF);
	}
	switch (mCmd[0]) {
	case 2:
		Drop();
		switch (mCmd[1]) {
		case 0x40:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = gGame.mXObjs.mPools[8].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					mHeldObjects[0] = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					if (mCmd[1] == 0x40)
						_C8C.mFlags |= 1;
				}
			}
			break;
		case 0x5B:
		case 0x7D:
		case 0x196:
		case 0x45B:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = gGame.mXObjs.mPools[22].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					mHeldObjects[0] = obj;
					switch (mCmd[1]) {
					case 0x5B:
						_C8C.mFlags |= 2;
						break;
					case 0x7D:
						_C8C.mFlags |= 4;
						break;
					case 0x196:
						_C8C.mFlags |= 8;
						break;
					case 0x45B:
						_C8C.mFlags |= 0x20;
						break;
					}
				}
			}
			break;
		case 0x1A1:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = gGame.mXObjs.mPools[22].Get(i);
				if (obj->GetKind() == 0x1A0) {
					obj->mStatus.mFlags |= 0x10000ULL;
					mHeldObjects[0] = obj;
				} else if (obj->GetKind() == 0x1A1) {
					obj->mStatus.mFlags |= 0x10000ULL;
					mHeldObjects[1] = obj;
				}
			}
			_C8C.mFlags |= 0x10;
			break;
		default:
			for (int i = 0; i < 2; ++i) {
				XObj*& held = mHeldObjects[i];
				if (held) {
					HSD_JObjClearMtxFlags(held->mJObj);
					held = NULL;
				}
			}
			break;
		}
		mCmd[0] = 0;
		break;
	case 3:
		switch (mCmd[1]) {
		case 1: {
			_998.mFlags |= 0x100;
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x40000000;
			player->mPad.mReset = FALSE;
			player->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);

			break;
		}
		default: {
			_998.mFlags &= ~0x100;
			s32 path[6] = { 0, 4, 2, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			pos   = CVec(jobj.GetPos());
			pos.y = mMoveTranslate.y;
			player->SetPos(pos, 0);
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x20000000;
			player->_980.mFlags |= 7;
			player->mPad.mReset = TRUE;
			break;
		}
		}
		mCmd[0] = 0;
		break;
	case 4:
		StartScreenShake(&gGame.mQuake, mCmd[1], mCmd[2], mCmd[3], 0.0f, 0.0f, 0.0f, mCmd[4] / 100.0f);
		mCmd[0] = 0;
		break;
	case 5: {
		void* joint = vfF0(mCmd[1]);
		void* mat   = vfF4(mCmd[2]);
		void* shape = vfF8(mCmd[1]);
		SetAnim(0, TRUE, joint, mat, shape, 0.0f, 1.0f, 1.0f, 10.0f);
		mStatus.mFlags |= 0x0008000000000000ULL;
		mCmd[0] = 0;
		break;
	}
	case 6: {
		s32 path[] = { 0, 4, 0, 0, 0 };
		jobj.Attach(FindJointByPath(mJObj, path, 5));
		pos = CVec(jobj.GetPos());
		pos.x += 8.0f * (f32)sin(HSD_JObjGetRotationY(mJObj));
		pos.y += 1.0f;
		pos.z += 8.0f * (f32)cos(HSD_JObjGetRotationY(mJObj));
		Vec rotation = { 0, 0, 0 };
		rotation.y   = HSD_JObjGetRotationY(mJObj);
		Vec scale    = { 1, 1, 1 };
		gGame.mParticle.CreateSRT(0x82, 1, pos, scale, rotation, TRUE);
		mCmd[0] = 0;
		break;
	}
	}
}

/**
 * @note Address: 0x8012926C
 * @note Size: 0xB0
 */
void XObjActorMama::vf15C()
{
	XObjChr* player = gGame.GetPlayer();
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPad.mReset = TRUE;
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
