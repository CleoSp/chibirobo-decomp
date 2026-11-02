#include "game/xobjactor.h"
#include "game/cjobjset.h"
#include "game/xchr.h"
#include "game/cscrn.h"
#include "game/globals.h"
#include "game/gmath.h"
#include "game/hsd_dobj.h"
extern "C" {
#include <sysdolphin/tobj.h>
}

extern "C" {
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);

// the sound tables the toys play along their anims
extern u8 lbl_8022C7E0[];

// the kinds' own updates
void fn_8011B7EC(XObjActorToy* object, u32 frames);
void fn_8011BE44(XObjActorToy* object, u32 frames);
void fn_8011C44C(XObjActorToy* object, u32 frames);
void fn_8011C834(XObjActorToy* object, u32 frames);
void fn_8011CF94(XObjActorToy* object, u32 frames);
}

// one of the toys in pool 7
static inline XObjActorToy* GetToy(s32 i)
{
	return (XObjActorToy*)lbl_8025C400.mXObjs.mPools[10].Get(i);
}

/**
 * @note Address: 0x80115538
 * @note Size: 0x24
 */
void XObjActorToy::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x8011555C
 * @note Size: 0x58
 */
void XObjActorToy::vf14C(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x801155B4
 * @note Size: 0xB8
 */
XObjActorToy::~XObjActorToy()
{
}

/**
 * @note Address: 0x8011566C
 * @note Size: 0x44
 */
bool XObjActorToy::IsFree() const
{
	if (!(mStatus.mFlags & 1)) {
		return true;
	}
	return !_998.Check(0x2000);
}

// the texture animation of the skip-th dobj's material
static inline HSD_AObj* GetTexAObj(HSD_JObj* jobj, s32 skip)
{
	if (jobj == NULL) {
		return NULL;
	}
	HSD_DObj* dobj = HSD_JObjGetDObj(jobj);
	for (s32 i = 0; i < skip; i++) {
		if (dobj == NULL) {
			return NULL;
		}
		dobj = HSD_DObjGetNext(dobj);
	}
	HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
	if (mobj == NULL) {
		return NULL;
	}
	HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
	if (tobj == NULL) {
		return NULL;
	}
	return tobj->aobj;
}

/**
 * @note Address: 0x801156B0
 * @note Size: 0x2D0
 */
void XObjActorToy::vf164(f32 frame)
{
	switch (GetKind()) {
	case 0x1FC:
		if (mAnimId == 0x3F6) {
			s32 path[3]    = { 0, 2, 0 };
			HSD_AObj* aobj = GetTexAObj(GetJointByPath(mJObj, path, 3), 4);
			frame          = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		}
		if (fn_8001E6C8(&lbl_8025C400.mSound, 0x10001F)) {
			if (mAnimId != 0x10) {
				fn_8001E660(&lbl_8025C400.mSound, 0x10001F);
			} else {
				fn_8001E468(&lbl_8025C400.mSound, 0x10001F, fn_8001DF00(&lbl_8025C400.mSound, &mMoveTranslate, 0), 0);
			}
		}
		break;
	case 0x159:
		if (fn_8001E6C8(&lbl_8025C400.mSound, 0xE0008) && mAnimId != 7) {
			fn_8001E660(&lbl_8025C400.mSound, 0xE0008);
		}
		break;
	}
	fn_80106BE0(this, frame);
}

/**
 * @note Address: 0x80115980
 * @note Size: 0x598
 */
// the 0x157 toy carries the 0x156 ones along, with what they hold in their hand
void XObjActorToy::vf130()
{
	switch (GetKind()) {
	case 0xF:
		break;
	case 0x6F:
		break;
	case 0x110:
		break;
	case 0x117:
		break;
	case 0x156:
		break;
	case 0x157:
		_2098.mFlags |= 2;
		for (s32 i = 0; i < 15; i++) {
			XObjActorToy* toy = GetToy(i);
			if (toy->GetKind() == 0x156 && (toy->_2098.mFlags & 1)) {
				toy->SetTranslateX(mMoveTranslate.x, 0);
				toy->SetTranslateZ(mMoveTranslate.z, 0);
				if (toy->_A4C >= 0 && toy->_A50 != NULL) {
					CJObj joint;
					s32 path[4] = { 0, 2, 4, 0 };
					Mtx hand;
					Mtx trans;
					Mtx rot;
					Mtx mtx;
					joint.Attach(GetJointFromRoot(toy->mJObj, 4, path));
					toy->_A50->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					PSMTXCopy(joint.GetMtx(), hand);
					PSMTXRotRad(rot, 'z', 0.0f);
					PSMTXConcat(hand, rot, mtx);
					PSMTXTrans(trans, 0.0f, 2.0f, 2.0f);
					PSMTXConcat(mtx, trans, mtx);
					HSD_JObj* jobj = toy->_A50->mJObj;
					MtxPtr m       = mtx;
					HSD_JObjCopyMtx(jobj, m);
					if (mStatus.mFlags & 0x10000) {
						toy->mStatus.mFlags |= 0x10000ULL;
					} else {
						toy->mStatus.mFlags &= ~0x10000ULL;
					}
				}
			}
		}
		break;
	case 0x159:
		break;
	case 0x1D3:
		break;
	case 0x1FB:
		break;
	case 0x1FC:
		break;
	case 0x25E:
		break;
	case 0x26C:
		break;
	case 0x26D:
		break;
	case 0x485:
		break;
	}
}

/**
 * @note Address: 0x80115F18
 * @note Size: 0xBC
 */
extern "C" s32 fn_80115F18(XObjActorToy* object, s32 count, s32* command)
{
	fn_801070A4(object, count, command);
	if (count < 1) {
		return 0;
	}
	object->mCmd[0] = command[0];
	if (count < 2) {
		object->mCmd[1] = 0;
	} else {
		object->mCmd[1] = command[1];
	}
	for (int i = 0; i < 8; i++) {
		object->mCmd[i + 2] = command[i + 2];
	}
	return 0;
}

/**
 * @note Address: 0x80119774
 * @note Size: 0x4B0
 */
// toy 0x25E, keeps the 0x341 objects it holds in its hand
// NONMATCHING: regalloc
extern "C" void fn_80119774(XObjActorToy* object, u32 frames)
{
	CJObj joint;
	if (object->_C84.mFlags & 0x40) {
		s32 path[1] = { 0 };
		CVec pos;
		joint.Attach(GetJointByPath(object->mJObj, path, 1));
		pos    = joint.GetPos();
		s32 id = -1;
		if (object->_C84.mFlags & 0x40) {
			id = 0x341;
		}
		if (id >= 0) {
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (id == obj->GetKind()) {
					MtxPtr mtx = HSD_JObjGetMtxPtr(joint.mJObj);
					HSD_JObjCopyMtx(obj->mJObj, mtx);
					if (object->mStatus.mFlags & 0x10000) {
						obj->mStatus.mFlags |= 0x10000ULL;
					} else {
						obj->mStatus.mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

/**
 * @note Address: 0x80119C24
 * @note Size: 0x42C
 */
// toy 0x26D, spins a joint once round
extern "C" void fn_80119C24(XObjActorToy* object, u32 frames)
{
	CJObj joint;
	switch (object->_990) {
	case 0:
		break;
	case 1:
		object->_990++;
		object->_988 = 0.0f;
		break;
	case 2: {
		object->_988 += 0.2f;
		s32 path[2] = { 0, 3 };
		joint.Attach(GetJointFromRoot(object->mJObj, 2, path));
		if (object->_988 >= TWO_PI) {
			object->_990 = 0;
			object->_988 = 0.0f;
		}
		joint.SetRotationY(object->_988 + HSD_JObjGetRotationY(object->mJObj), 0);
		break;
	}
	}
}

/**
 * @note Address: 0x8011A050
 * @note Size: 0x340
 */
// toys 0x26C and 0x485
extern "C" void fn_8011A050(XObjActorToy* object, u32 frames)
{
	CJObj joint;
	switch (object->_990) {
	case 0:
		if (object->_9A4 > 0) {
			object->_990++;
		}
		break;
	case 1: {
		s32 path[4] = { 0, 2, 4, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path));
		joint.SetTranslateY(-object->_9A8, 0);
		if (1.0f == object->_9A8) {
			object->_990 = 0;
		}
		break;
	}
	}
}

/**
 * @note Address: 0x8011A390
 * @note Size: 0x1338
 */
// toy 0x1FC, pushes two of its joints out and back
// NONMATCHING: the paths sit at other .rodata offsets, the CJObj ctor is inlined
extern "C" void fn_8011A390(XObjActorToy* object, u32 frames)
{
	CJObj joint;
	static f32 sTime = 0.0f;
	static Vec sPos0;
	static Vec sPos1;
	switch (object->_990) {
	case 1: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path0));
		sPos0.x      = HSD_JObjGetTranslationX(joint.mJObj);
		sPos0.y      = HSD_JObjGetTranslationY(joint.mJObj);
		sPos0.z      = HSD_JObjGetTranslationZ(joint.mJObj);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 5, path1));
		sPos1.x = HSD_JObjGetTranslationX(joint.mJObj);
		sPos1.y = HSD_JObjGetTranslationY(joint.mJObj);
		sPos1.z = HSD_JObjGetTranslationZ(joint.mJObj);
		object->_990++;
		break;
	}
	case 2: {
		sTime += 0.16f;
		if (sTime >= 1.0f) {
			object->_990++;
		}
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		break;
	}
	case 3: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		sTime -= 0.16f;
		if (sTime <= 0.0f) {
			object->_990 = 0;
			s32 path2[4] = { 0, 2, 0, 1 };
			joint.Attach(GetJointFromRoot(object->mJObj, 4, path2));
			joint.SetTranslateX(sPos0.x, 0);
			joint.SetTranslateY(sPos0.y, 0);
			joint.SetTranslateZ(sPos0.z, 0);
			s32 path3[5] = { 0, 2, 0, 0, 0 };
			joint.Attach(GetJointFromRoot(object->mJObj, 5, path3));
			joint.SetTranslateX(sPos1.x, 0);
			joint.SetTranslateY(sPos1.y, 0);
			joint.SetTranslateZ(sPos1.z, 0);
			sTime = 0.0f;
		}
		break;
	}
	}
	if (object->_2098.mFlags & 1) {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path0));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 5, path1));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 6, path2));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 7, path3));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
	} else {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(object->mJObj, 4, path0));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 5, path1));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 6, path2));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(object->mJObj, 7, path3));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
	}
}

/**
 * @note Address: 0x8011E3A4
 * @note Size: 0x2E4
 */
// hides the item the toy dropped, the _C84 bits say which
extern "C" void fn_8011E3A4(XObjActorToy* object)
{
	s32 id = -1;
	if (object->_C84.Check(1)) {
		id = 0x3D;
		object->_C84.mFlags &= ~1;
	} else if (object->_C84.Check(2)) {
		id = 0x3E;
		object->_C84.mFlags &= ~2;
	} else if (object->_C84.Check(4)) {
		id = 0x3F;
		object->_C84.mFlags &= ~4;
	} else if (object->_C84.Check(8)) {
		id = 0x3B;
		object->_C84.mFlags &= ~8;
	} else if (object->_C84.Check(0x80)) {
		id = 0x21D;
		object->_C84.mFlags &= ~0x80;
	} else if (object->_C84.Check(0x10)) {
		id = 0x21E;
		object->_C84.mFlags &= ~0x10;
	} else if (object->_C84.Check(0x40)) {
		id = 0x341;
		object->_C84.mFlags &= ~0x40;
	}
	if (id < 0) {
		return;
	}
	switch (id) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
	case 0x21D:
	case 0x21E:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
			if (id == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x341:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
			if (id == obj->GetKind()) {
				obj->mStatus.mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x8011E688
 * @note Size: 0x254
 */
extern "C" void fn_8011E688(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		object->_A4C   = object->mCmd[1];
		object->mState = &XObjActorToy::StatePickUp;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 2:
		object->_A4C = -1;
		if (object->_A50 != NULL) {
			object->_A50->Release();
		}
		object->mCmd[0] = 0;
		break;
	case 3:
		fn_8011E3A4(object);
		switch (object->mCmd[1]) {
		case 0x341:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (object->mCmd[1] == obj->GetKind()) {
					object->_A84 = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					switch (object->mCmd[1]) {
					case 0x341:
						object->_C84.mFlags |= 0x40;
						break;
					}
				}
			}
			break;
		default:
			if (object->_A84 != NULL) {
				HSD_JObjClearMtxFlags(object->_A84->mJObj);
				object->_A84 = NULL;
			}
			break;
		}
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011E8DC
 * @note Size: 0xD0
 */
extern "C" void fn_8011E8DC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		fn_800242A8(&lbl_8025C400.mQuake, object->mCmd[1], object->mCmd[2], object->mCmd[3], 0.0f, 0.0f, 0.0f, object->mCmd[4] / 100.0f);
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011E9AC
 * @note Size: 0x140
 */
extern "C" void fn_8011E9AC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		object->mCmd[0] = 0;
		break;
	case 1:
		fn_800242A8(&lbl_8025C400.mQuake, object->mCmd[1], object->mCmd[2], object->mCmd[3], 0.0f, 0.0f, 0.0f, object->mCmd[4] / 100.0f);
		object->mCmd[0] = 0;
		break;
	case 2:
		switch (object->mCmd[1]) {
		case 0:
			object->_2098.mFlags |= 1;
			object->mCmd[0] = 0;
			break;
		case 1:
			object->_2098.mFlags &= ~1;
			object->mCmd[0] = 0;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8011EAEC
 * @note Size: 0xAC
 */
extern "C" void fn_8011EAEC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		object->_A4C   = object->mCmd[1];
		object->mState = &XObjActorToy::StatePickUp;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 2:
		object->_A4C = -1;
		if (object->_A50 != NULL) {
			object->_A50->Release();
		}
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011EB98
 * @note Size: 0x80
 */
extern "C" void fn_8011EB98(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		object->_2098.mFlags |= 8;
		object->mCmd[0] = 0;
		break;
	case 2:
		if (object->mStatus.mFlags & 0x10000) {
			lbl_80367A2C[object->mCmd[1]] = 1;
		}
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011EC18
 * @note Size: 0x130
 */
extern "C" void fn_8011EC18(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		object->mCmd[0] = 0;
		break;
	case 2:
		if (object->mMoveTranslate.y <= 2.0f) {
			lbl_80367A2C[object->mCmd[1]] = 1;
		}
		object->mCmd[0] = 0;
		break;
	case 3:
		if (object->_2098.mFlags & 0x10) {
			lbl_80367A2C[object->mCmd[1]] = 1;
		}
		object->mCmd[0] = 0;
		break;
	case 4:
		object->_A4C   = object->mCmd[1];
		object->mState = &XObjActorToy::StatePickUp;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 5:
		object->_A4C = -1;
		if (object->_A50 != NULL) {
			object->_A50->Release();
		}
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011ED48
 * @note Size: 0x144
 */
extern "C" void fn_8011ED48(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		switch (object->mCmd[1]) {
		case 0: {
			object->_2098.mFlags &= ~1;
			object->_984 = 0.0f;
			object->_990 = 0;
			CVec pos(object->mMoveTranslate.x, object->mMoveTranslate.y, object->mMoveTranslate.z);
			object->SetMoveDir(&pos, 0.0f);
			break;
		}
		case 1:
			object->_2098.mFlags |= 1;
			object->_9C8.x = object->mCmd[2] / 100.0f;
			object->_9C8.y = object->mCmd[3] / 100.0f;
			object->_9C8.z = object->mCmd[4] / 100.0f;
			object->_990   = 1;
			break;
		}
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011EE8C
 * @note Size: 0x410
 */
// NONMATCHING: regalloc
extern "C" void fn_8011EE8C(XObjActorToy* object)
{
	s32 count;
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		switch (object->mCmd[1]) {
		case 0:
			object->_2098.mFlags &= ~1;
			break;
		case 1:
			object->_2098.mFlags |= 1;
			break;
		}
		object->mCmd[0] = 0;
		break;
	case 2:
		fn_8011E3A4(object);
		switch (object->mCmd[1]) {
		case 0x3B:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x21D:
		case 0x21E:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[8].Get(i);
				if (object->mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					object->_A84 = obj;
					switch (object->mCmd[1]) {
					case 0x3B:
						object->_C84.mFlags |= 8;
						break;
					case 0x3D:
						object->_C84.mFlags |= 1;
						break;
					case 0x3E:
						object->_C84.mFlags |= 2;
						break;
					case 0x3F:
						object->_C84.mFlags |= 4;
						break;
					case 0x21D:
						object->_C84.mFlags |= 0x80;
						break;
					case 0x21E:
						object->_C84.mFlags |= 0x10;
						break;
					}
				}
			}
			break;
		default:
			if (object->_A84 != NULL) {
				HSD_JObjClearMtxFlags(object->_A84->mJObj);
				object->_A84 = NULL;
			}
			break;
		}
		object->mCmd[0] = 0;
		break;
	case 3:
		lbl_80367A2C[object->mCmd[1]] = fn_80106B50();
		object->mCmd[0]               = 0;
		break;
	case 4:
		count = 0;
		for (s32 i = 0; i < 0x100; i++) {
			InventoryItem* items = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
			s16 num              = items[i].mCount;
			s16 id               = items[i].mId;
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		lbl_8025C400.mEvt._10 = count;
		object->mCmd[0]       = 0;
		break;
	case 5:
		object->_A4C   = object->mCmd[1];
		object->mState = &XObjActorToy::StatePickUp;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 6:
		object->_A4C = -1;
		if (object->_A50 != NULL) {
			object->_A50->Release();
		}
		object->mCmd[0] = 0;
		break;
	case 7:
		if (object->mCmd[1]) {
			object->mPieces[0].mId    = object->mCmd[2];
			object->mPieces[0].mState = 1;
		} else {
			for (u32 j = 0; j < 4; j++) {
				if (object->mPieces[j].mObj != NULL) {
					object->mPieces[j].mObj->Release();
				}
				object->mPieces[j].mId    = -1;
				object->mPieces[j].mState = 0;
				object->mPieces[j].mObj   = NULL;
			}
		}
		object->mCmd[0] = 0;
		break;
	case 8:
		lbl_80367A2C[object->mCmd[1]] = fn_80106AC0();
		object->mCmd[0]               = 0;
		break;
	}
}

/**
 * @note Address: 0x8011F29C
 * @note Size: 0x220
 */
// NONMATCHING: regalloc
void XObjActorToy::vf148(u32 frames)
{
	fn_80107750(this, frames);
	switch (GetKind()) {
	case 0xF:
		fn_8011CF94(this, frames);
		break;
	case 0x6F:
		fn_8011C834(this, frames);
		break;
	case 0x110:
		break;
	case 0x117:
		fn_8011C44C(this, frames);
		break;
	case 0x156:
		if (_2098.mFlags & 1) {
			for (s32 i = 0; i < 15; i++) {
				if (GetToy(i)->GetKind() == 0x157) {
					Vec pos          = GetToy(i)->mMoveTranslate;
					mMoveTranslate.x = pos.x;
					mMoveTranslate.y = pos.y;
					mMoveTranslate.z = pos.z;
				}
			}
		}
		break;
	case 0x158:
		break;
	case 0x159:
		fn_8011BE44(this, frames);
		break;
	case 0x16C:
		break;
	case 0x1D3:
		break;
	case 0x1FB:
		fn_8011B7EC(this, frames);
		break;
	case 0x1FC:
		fn_8011A390(this, frames);
		break;
	case 0x26C:
	case 0x485:
		fn_8011A050(this, frames);
		break;
	case 0x26D:
		fn_80119C24(this, frames);
		break;
	case 0x25E:
		fn_80119774(this, frames);
		break;
	}
}

/**
 * @note Address: 0x801219B0
 * @note Size: 0x104
 */
// distance between two points on the floor
// NONMATCHING: the inlined sqrtf
extern "C" f32 fn_801219B0(const Vec* first, const Vec* second)
{
	f32 x       = first->x - second->x;
	f32 z       = first->z - second->z;
	f32 squared = 0.0f;
	squared += x * x;
	return sqrtf(z * z + squared);
}

/**
 * @note Address: 0x801220A8
 * @note Size: 0xA4
 */
void XObjActorToy::vfC8()
{
	fn_80107600(this);
	if (mFirst.mStatus.mFlags & 0x10000) {
		mFirst.vfC8();
	}
	if (mSecond.mStatus.mFlags & 0x10000) {
		mSecond.vfC8();
	}
}

/**
 * @note Address: 0x8012214C
 * @note Size: 0x82C
 */
void XObjActorToy::vfAC()
{
	fn_80107620(this);
	if (mFirst.mStatus.mFlags & 0x10000) {
		mFirst.CJObj::SetTranslate(GetTranslateX(), GetTranslateY(), GetTranslateZ(), 1);
		mFirst.CJObj::SetScale(GetScaleX(), GetScaleY(), GetScaleZ(), 1);
		mFirst.CJObj::SetRotationY(GetRotationY(), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mFirst.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mFirst.mJObj, 8);
		}
		mFirst.vfAC();
	}
	if (mSecond.mStatus.mFlags & 0x10000) {
		mSecond.CJObj::SetTranslate(GetTranslateX(), GetTranslateY(), GetTranslateZ(), 1);
		mSecond.CJObj::SetScale(GetScaleX(), GetScaleY(), GetScaleZ(), 1);
		mSecond.CJObj::SetRotationY(GetRotationY(), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mSecond.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mSecond.mJObj, 8);
		}
		mSecond.vfAC();
	}
}

/**
 * @note Address: 0x80122978
 * @note Size: 0x64
 */
void XObjActorToy::Release()
{
	mFirst.Release();
	mSecond.Release();
	fn_800911C0(this);
	mLoad = 0;
	_1F90 = 0;
}

/**
 * @note Address: 0x801229DC
 * @note Size: 0x1E8
 */
BOOL XObjActorToy::vf9C(const CoordInfo& info)
{
	switch (GetKind()) {
	case 0xF:
		switch (mLoad) {
		case 0:
			if (!fn_80107F18(this, info)) {
				break;
			}
			mLoad++;
		case 1:
			if (!mFirst.vf9C(CoordInfo(GetSpawnData(0x2A)))) {
				break;
			}
			mFirst.mStatus.mFlags &= ~0x10000ULL;
			mFirst.SetPos(mMoveTranslate, 0);
			mLoad++;
		case 2:
			if (!mSecond.vf9C(CoordInfo(GetSpawnData(0x2B)))) {
				break;
			}
			mSecond.mStatus.mFlags &= ~0x10000ULL;
			mSecond.SetPos(mMoveTranslate, 0);
			mLoad++;
		}
		if (mLoad > 2) {
			mLoad = 0;
			return TRUE;
		}
		return FALSE;
	}
	return fn_80107F18(this, info) != FALSE;
}

/**
 * @note Address: 0x80122BC4
 * @note Size: 0x174
 */
void XObjActorToy::vf98()
{
	u8* data = lbl_8022C7E0;
	fn_801081F0(this);
	mLoad = 0;
	_1F90 = 0;
	mFirst.vf98();
	mSecond.vf98();
	switch (GetKind()) {
	case 0x6F:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x374);
		break;
	case 0x110:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x68);
		break;
	case 0x117:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x50C);
		break;
	case 0x159:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x4DC);
		break;
	case 0x1FB:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x62C);
		break;
	case 0x1FC:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x3EC);
		break;
	case 0x25E:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x74C);
		break;
	case 0x317:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x68C);
		break;
	case 0x26C:
	case 0x485:
		mAnimSfx.mEvents = (AnimSfxEvent*)(data + 0x4AC);
		break;
	default:
		mAnimSfx.mEvents = NULL;
		break;
	}
}

/**
 * @note Address: 0x80122D38
 * @note Size: 0x60
 */
void XObjActorToy::vf94()
{
	fn_80108014(this);
	mLoad = 0;
	_1F90 = 0;
	mFirst.vf94();
	mSecond.vf94();
}

/**
 * @note Address: 0x80122D98
 * @note Size: 0x74
 */
void XObjActorToy::vf90(u32 id)
{
	fn_801083F0(this, id);
	mLoad = 0;
	_1F90 = 0;
	mFirst.vf90(id);
	mSecond.vf90(id);
}
