#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

extern "C" {
void HSD_MtxGetScale(Mtx mtx, Vec* scale);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
CVec fn_800621E0(XObj* object); // the object's scale as it is drawn
}

/**
 * @note Address: 0x8012C858
 * @note Size: 0x24
 */
void XObjActorTao::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x8012C87C
 * @note Size: 0x58
 */
void XObjActorTao::vf14C(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x8012C8D4
 * @note Size: 0x78
 */
XObjActorTao::~XObjActorTao()
{
}

/**
 * @note Address: 0x8012C94C
 * @note Size: 0xAC
 */
HSD_JObj* XObjActorTao::vf168()
{
	s32 path[4] = { 0, 0, 0, 0 };
	return FindJointByPath(mJObj, path, 4);
}

/**
 * @note Address: 0x8012C9F8
 * @note Size: 0x5C
 */
CVec XObjActorTao::vf16C()
{
	s32 path[4] = { 0, 0, 0, 0 };
	return fn_8007E7C8(this, 4, path);
}

/**
 * @note Address: 0x8012CA54
 * @note Size: 0x44
 */
bool XObjActorTao::IsFree()
{
	if ((mStatus.mFlags & 1) == 0) {
		return true;
	}
	return !(_998.mFlags & 0x2000);
}

/**
 * @note Address: 0x8012CA98
 * @note Size: 0x770
 */
s32 XObjActorTao::vf124(s32 count, s32* command)
{
	vf124__9XObjActorFlPl(this, count, command);
	if (count == 0) {
		return -1;
	}
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			fn_8010771C(this, 0.08726646f);
			break;
		case 1:
			fn_801076B0(this, (s32)&lbl_8025C400.GetPlayer()->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(command[2]) != NULL) {
				fn_801076FC(this, (s32)&FindObj(command[2])->mMoveTranslate, DEG2RAD * command[3]);
			}
			break;
		case 3:
			fn_801076DC(this, DEG2RAD * command[2], DEG2RAD * command[3]);
			break;
		}
		break;
	case 2:
		Drop();
		switch (command[1]) {
		case 0x102:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
				if (command[1] == obj->GetKind()) {
					_A84 = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					switch (command[1]) {
					case 0x102:
						_C8C.mFlags |= 2;
						break;
					}
				}
			}
			break;
		case 0x159:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[10].Get(i);
				if (command[1] == obj->GetKind()) {
					_A84 = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 1;
				}
			}
			break;
		case 0xF8: {
			mCmd[2] = command[2];
			s32 n   = -1;
			for (s32 i = 0; i < 21; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[16].Get(i);
				if (command[1] == obj->GetKind()) {
					if (++n == mCmd[2]) {
						obj->mStatus.mFlags |= 0x10000ULL;
						_A84 = obj;
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		}
		case 0x2A3:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (command[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 8;
					_A84 = obj;
				}
			}
			break;
		case 0x46F:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (command[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 0x20;
					_A84 = obj;
				}
			}
			break;
		default:
			if (_A84 != NULL) {
				HSD_JObjClearMtxFlags(_A84->mJObj);
				_A84 = NULL;
			}
			break;
		}
		break;
	case 3:
		_A4C   = command[1];
		mState = &XObjActorTao::StatePickUp;
		fn_8010706C(this);
		mCmd[0] = 0;
		break;
	case 4:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		mCmd[0] = 0;
		break;
	case 5:
		if (command[1] != 0) {
			_CF4 = 0x82;
			_CF8 = 1;
		} else {
			_CF8 = 0;
			_CF4 = -1;
			if (_CFC != NULL) {
				_CFC->Release();
			}
		}
		mCmd[0] = 0;
		break;
	default:
		if (count < 1) {
			return 0;
		}
		mCmd[0] = command[0];
		if (count < 2) {
			mCmd[1] = 0;
		} else {
			mCmd[1] = command[1];
		}
		mCmd[2] = command[2];
		mCmd[3] = command[3];
		mCmd[4] = command[4];
		mCmd[5] = command[5];
		mCmd[6] = command[6];
		mCmd[7] = command[7];
		mCmd[8] = command[8];
		mCmd[9] = command[9];
		break;
	}
	return 0;
}

/**
 * @note Address: 0x8012D86C
 * @note Size: 0x4
 */
void XObjActorTao::StateNone(u32 frames)
{
}

/**
 * @note Address: 0x8012D208
 * @note Size: 0x664
 */
// picks up the object _A4C names and holds it in its mouth
void XObjActorTao::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0:
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		_990++;
	case 1:
		if (_A50 != NULL) {
			if (_A50->mStatus.mFlags & 0x0040000000000000ULL) {
				return;
			}
			lbl_8025C400.mStatus.mFlags |= 0x1000;
		}
		_990++;
	case 2:
		if (_A4C >= 0 && _A50 != NULL) {
			_A50->SetScale(1.0f, 1.0f, 1.0f, 0);
			CJObj jobj;
			s32 path[6] = { 0, 0, 0, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos = jobj.GetPos();
			_A50->SetPos(pos.x, HSD_JObjGetTranslationY(mJObj), pos.z, 0);
			Vec scale;
			PSMTXCopy(jobj.GetMtx(), _C94);
			HSD_MtxGetScale(_C94, &scale);
			scale.x = fn_800621E0(_A50).x / scale.x;
			scale.y = fn_800621E0(_A50).y / scale.y;
			scale.z = fn_800621E0(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat(_C94, scaleMtx, m);
			PSMTXTrans(transMtx, 0.0f, -10.0f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);
			_A50->mStatus.mFlags &= ~0x0200000000000000ULL;
		}
		break;
	}
}

/**
 * @note Address: 0x8012EFF4
 * @note Size: 0x320
 */
// lets go of what it carries
// NONMATCHING: 0x159 and 0xF8 still walk their pools
void XObjActorTao::Drop()
{
	s32 kind = -1;
	if (_C8C.Check(2)) {
		_C8C.mFlags &= ~2;
		kind = 0x102;
	} else if (_C8C.Check(1)) {
		_C8C.mFlags &= ~1;
		kind = 0x159;
	} else if (_C8C.Check(4)) {
		_C8C.mFlags &= ~4;
		kind = 0xF8;
	} else if (_C8C.Check(8)) {
		_C8C.mFlags &= ~8;
		kind = 0x2A3;
	} else if (_C8C.Check(0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x46F;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x102:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x159:
		break;
	case 0xF8:
		break;
	case 0x2A3:
	case 0x46F:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x8012F314
 * @note Size: 0x50
 */
void XObjActorTao::vf158()
{
	if (_998.Check(2)) {
		_998.mFlags &= ~2;
		fn_8001E904(&lbl_8025C400.mSound, 0x15D, 0xFF);
	}
}

/**
 * @note Address: 0x8012F364
 * @note Size: 0xC4
 */
void XObjActorTao::vf15C()
{
	_998.mFlags = 0;
	_C8C.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0;
	mState      = &XObjActorTao::StateNone;
	fn_8010706C(this);
	mCmd[2] = 0;
	mCmd[3] = 0;
	mCmd[4] = 0;
	mCmd[5] = 0;
	mCmd[6] = 0;
	mCmd[7] = 0;
	mCmd[8] = 0;
	mCmd[9] = 0;
	_CF4    = -1;
	_CF8    = 0;
	_CFC    = NULL;
	_CF4    = 0x82;
	_CF8    = 1;
	_998.mFlags |= 0x2000;
}
