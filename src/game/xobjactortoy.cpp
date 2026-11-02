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

// the kinds' own updates
void fn_8011B7EC(XObjActorToy* object, u32 frames);
void fn_8011BE44(XObjActorToy* object, u32 frames);
void fn_8011C44C(XObjActorToy* object, u32 frames);
void fn_8011C834(XObjActorToy* object, u32 frames);
void fn_8011CF94(XObjActorToy* object, u32 frames);
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
