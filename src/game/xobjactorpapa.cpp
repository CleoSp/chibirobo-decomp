#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cscrn.h"
#include "game/gmath.h"
#include <stdlib.h>

extern "C" {
void HSD_MtxGetScale(Mtx mtx, Vec* scale);
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
CVec fn_800621E0(XObj* object); // the object's scale as it is drawn
}

// takes the controls away from the player
static inline void LockPlayer(XObjChr* player)
{
	player->_980.mFlags = 0;
	player->_980.mFlags |= 0x40000000;
	player->mPadReset = FALSE;
}

/**
 * @note Address: 0x8012931C
 * @note Size: 0x24
 */
void XObjActorPapa::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x80129340
 * @note Size: 0x58
 */
void XObjActorPapa::vf14C(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80129398
 * @note Size: 0x78
 */
XObjActorPapa::~XObjActorPapa()
{
}

/**
 * @note Address: 0x80129410
 * @note Size: 0xA4
 */
HSD_JObj* XObjActorPapa::vf168()
{
	s32 path[3] = { 0, 2, 0 };
	return FindJointByPath(mJObj, path, 3);
}

/**
 * @note Address: 0x801294B4
 * @note Size: 0x54
 */
CVec XObjActorPapa::vf16C()
{
	s32 path[3] = { 0, 2, 0 };
	return fn_8007E7C8(this, 3, path);
}

/**
 * @note Address: 0x80129508
 * @note Size: 0x44
 */
bool XObjActorPapa::IsFree()
{
	if ((mStatus.mFlags & 1) == 0) {
		return true;
	}
	return !(_998.mFlags & 0x2000);
}

/**
 * @note Address: 0x8012954C
 * @note Size: 0x8
 */
f32 XObjActorPapa::vf17C()
{
	return -0.17453292f;
}

/**
 * @note Address: 0x80129554
 * @note Size: 0x8
 */
f32 XObjActorPapa::vf178()
{
	return 0.17453292f;
}

/**
 * @note Address: 0x8012955C
 * @note Size: 0x210
 */
s32 XObjActorPapa::vf124(s32 count, s32* command)
{
	vf124__9XObjActorFlPl(this, count, command);
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
 * @note Address: 0x8012976C
 * @note Size: 0x76C
 */
// picks up the object _A4C names and holds it in his hand
void XObjActorPapa::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0: {
		XObjActorTao* tao = ((XObjActorTao*)lbl_8025C400.mXObjs.mPools[14].mObjects);
		if ((tao->mStatus.mFlags & 1) && !tao->IsFree()) {
			return;
		}
		bool busy = false;
		for (s32 i = 0; i < 15; i++) {
			XObjActorToy* toy = (XObjActorToy*)lbl_8025C400.mXObjs.mPools[10].Get(i);
			if (toy->GetKind() == 0x159) {
				if (!(toy->mStatus.mFlags & 1)) {
					break;
				}
				if (!toy->IsFree()) {
					busy = true;
					break;
				}
			}
		}
		if (busy) {
			return;
		}
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		_998.mFlags |= 0x2000;
		_990++;
	}
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
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos;
			pos = GetJointPosition(jobj.mJObj);
			Vec scale;
			scale.x = pos.x;
			scale.y = pos.y;
			scale.z = pos.z;
			_A50->SetPos(pos.x, pos.y + 2.0f, pos.z, 0);
			PSMTXCopy(jobj.GetMtx(), _A54);
			HSD_MtxGetScale(_A54, &scale);
			scale.x = fn_800621E0(_A50).x / scale.x;
			scale.y = fn_800621E0(_A50).y / scale.y;
			scale.z = fn_800621E0(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx rotMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat(_A54, scaleMtx, m);
			PSMTXRotRad(rotMtx, 'z', 1.5707964f);
			PSMTXConcat(m, rotMtx, m);
			PSMTXTrans(transMtx, -5.5f, 3.5f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);

			_A50->mStatus.mFlags &= ~0x0200000000000000ULL;
		}
		_998.mFlags &= ~0x2000;
		break;
	}
}

/**
 * @note Address: 0x80129ED8
 * @note Size: 0x4
 */
void XObjActorPapa::StateNone(u32 frames)
{
}

/**
 * @note Address: 0x8012BB5C
 * @note Size: 0x288
 */
// lets go of what he carries
// NONMATCHING: 0x1D3 and 0x375 still walk their pools
void XObjActorPapa::Drop()
{
	s32 kind = -1;
	if (_C8C.Check(1)) {
		_C8C.mFlags &= ~1;
		kind = 0x1F4;
	} else if (_C8C.Check(2)) {
		_C8C.mFlags &= ~2;
		kind = 0x360;
	} else if (_C8C.Check(4)) {
		_C8C.mFlags &= ~4;
		kind = 0x1D3;
	} else if (_C8C.Check(8)) {
		_C8C.mFlags &= ~8;
		kind = 0x375;
	} else if (_C8C.Check(0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x3FA;
	} else if (_C8C.Check(0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x401;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x1D3:
		break;
	case 0x1F4:
	case 0x360:
	case 0x3FA:
	case 0x401:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (kind == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x375:
		break;
	}
}

/**
 * @note Address: 0x8012BDE4
 * @note Size: 0x9BC
 */
void XObjActorPapa::vf158()
{
	CJObj jobj;
	if (_998.Check(0x400)) {
		_998.mFlags &= ~0x400;
		_AB0 = 0.0f;
	}
	if (_998.Check(2)) {
		_998.mFlags &= ~2;
		fn_8001E904(&lbl_8025C400.mSound, rand() % 10 + 0xB7, 0xFF);
	}
	switch (mCmd[0]) {
	case 2:
		Drop();
		switch (mCmd[1]) {
		case 0x1F4:
		case 0x360:
		case 0x3FA:
		case 0x401:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					_A84 = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					switch (mCmd[1]) {
					case 0x1F4:
						_C8C.mFlags |= 1;
						break;
					case 0x360:
						_C8C.mFlags |= 2;
						break;
					case 0x3FA:
						_C8C.mFlags |= 0x10;
						break;
					case 0x401:
						_C8C.mFlags |= 0x20;
						break;
					}
				}
			}
			break;
		case 0x1D3:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[10].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_A84 = obj;
					switch (mCmd[1]) {
					case 0x1D3:
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		case 0x375:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_A84 = obj;
					switch (mCmd[1]) {
					case 0x375:
						_C8C.mFlags |= 8;
						break;
					}
				}
			}
			break;
		default:
			if (_A84 != NULL) {
				HSD_JObj* held = _A84->mJObj;
				HSD_JObjClearMtxFlags(held);
				_A84 = NULL;
			}
			break;
		}
		mCmd[0] = 0;
		break;
	case 3:
		switch (mCmd[1]) {
		case 1: {
			_998.mFlags |= 0x100;
			LockPlayer(lbl_8025C400.GetPlayer());
			lbl_8025C400.GetPlayer()->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);

			break;
		}
		default: {
			_998.mFlags &= ~0x100;
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos;
			pos = GetJointPosition(jobj.mJObj);
			Vec target;
			target.x = pos.x;
			target.y = pos.y;
			target.z = pos.z;
			target.y = mMoveTranslate.y;
			lbl_8025C400.GetPlayer()->SetPos(target, 0);
			XObjChr* player     = lbl_8025C400.GetPlayer();
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x20000000;
			player->_980.mFlags |= 7;
			player->mPadReset = TRUE;
			break;
		}
		}
		mCmd[0] = 0;
		break;
	case 4:
		fn_800242A8(&lbl_8025C400.mQuake, mCmd[1], mCmd[2], mCmd[3], 0.0f, 0.0f, 0.0f, mCmd[4] / 100.0f);
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
	case 6:
		_A4C   = mCmd[1];
		mState = &XObjActorPapa::StatePickUp;
		fn_8010706C(this);
		mCmd[0] = 0;
		break;
	case 7:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		mCmd[0] = 0;
		break;
	case 8:
		switch (mCmd[1]) {
		case 1:
			_C94 = 1;
			break;
		default:
			_C94 = 0;
			break;
		}
		mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8012C7A0
 * @note Size: 0xB8
 */
void XObjActorPapa::vf15C()
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
	_C94        = 1;
	mState      = &XObjActorPapa::StateNone;
	fn_8010706C(this);
}
