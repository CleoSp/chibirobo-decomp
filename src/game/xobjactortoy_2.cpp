#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactortoy.h"

// the texture animation of the jobj's dobj after skip others
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

// one of the toys, see GAME_POOL_975C
static inline XObjActorToy* GetToy(s32 i)
{
	return (XObjActorToy*)GAME_POOL_975C->Get(i);
}

static inline MtxPtr ToyMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

extern "C" const u8 lbl_8022C7E0[];
extern "C" {
void fn_80119774(XObjActorToy* self, u32 frames);
void fn_80119C24(XObjActorToy* self, u32 frames);
void fn_8011A050(XObjActorToy* self, u32 frames);
void fn_8011A390(XObjActorToy* self, u32 frames);
void fn_8011B7EC(XObjActorToy* self, u32 frames);
void fn_8011BE44(XObjActorToy* self, u32 frames);
void fn_8011C44C(XObjActorToy* self, u32 frames);
void fn_8011C834(XObjActorToy* self, u32 frames);
void fn_8011CF94(XObjActorToy* self, u32 frames);
}

static inline s16 ToyItemId(const UnkPlayerItem& item)
{
	return item.mId;
}

static inline s16 ToyItemNum(const UnkPlayerItem& item)
{
	return item.mNum;
}

// toys 0x26C and 0x485
extern "C" void fn_8011A050(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	switch (self->_990) {
	case 0:
		if (self->_9A4 > 0) {
			self->_990++;
		}
		break;
	case 1: {
		s32 path[4] = { 0, 2, 4, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path));
		joint.SetTranslateY(-self->_9A8, 0);
		if (1.0f == self->_9A8) {
			self->_990 = 0;
		}
		break;
	}
	}
}

// toy 0x25E, keeps the 0x341 objects it holds in its hand
// NONMATCHING: register allocation
extern "C" void fn_80119774(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	if (self->_C84.mFlags & 0x40) {
		static const s32 path[1] = { 0 };
		CVec pos;
		joint.Attach(GetJointByPath(self->mJObj, (s32*)path, 1));
		pos    = CVec(GetJObjPos(joint.mJObj));
		s32 id = -1;
		if (self->_C84.mFlags & 0x40) {
			id = 0x341;
		}
		if (id >= 0) {
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (id == obj->GetDescKind()) {
					MtxPtr mtx = HSD_JObjGetMtxPtr(joint.mJObj);
					HSD_JObjCopyMtx(obj->mJObj, mtx);
					if (self->mFlags & 0x10000) {
						obj->mFlags |= 0x10000ULL;
					} else {
						obj->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

static inline f32 ToyTranslationX(const CJObj& obj)
{
	return HSD_JObjGetTranslationX(obj.mJObj);
}

static inline f32 ToyTranslationY(const CJObj& obj)
{
	return HSD_JObjGetTranslationY(obj.mJObj);
}

static inline f32 ToyTranslationZ(const CJObj& obj)
{
	return HSD_JObjGetTranslationZ(obj.mJObj);
}

static inline f32 ToyScaleX(const CJObj& obj)
{
	return HSD_JObjGetScaleX(obj.mJObj);
}

static inline f32 ToyScaleY(const CJObj& obj)
{
	return HSD_JObjGetScaleY(obj.mJObj);
}

static inline f32 ToyScaleZ(const CJObj& obj)
{
	return HSD_JObjGetScaleZ(obj.mJObj);
}

static inline f32 ToyRotationY(const CJObj& obj)
{
	return HSD_JObjGetRotationY(obj.mJObj);
}

// toy 0x26D, spins a joint once round
extern "C" void fn_80119C24(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	switch (self->_990) {
	case 0:
		break;
	case 1:
		self->_990++;
		self->_988 = 0.0f;
		break;
	case 2: {
		self->_988 += 0.2f;
		s32 path[2] = { 0, 3 };
		joint.Attach(GetJointFromRoot(self->mJObj, 2, path));
		if (self->_988 >= 6.2831855f) {
			self->_990 = 0;
			self->_988 = 0.0f;
		}
		joint.SetRotationY(self->_988 + HSD_JObjGetRotationY(self->mJObj), 0);
		break;
	}
	}
}

// toy 0x1FC, pushes two of its joints out and back
// NONMATCHING: the path templates sit at other .rodata offsets and retail inlines the CJObj
// constructor
extern "C" void fn_8011A390(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	static f32 sTime = 0.0f;
	static Vec sPos0;
	static Vec sPos1;
	switch (self->_990) {
	case 1: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		sPos0.x      = HSD_JObjGetTranslationX(joint.mJObj);
		sPos0.y      = HSD_JObjGetTranslationY(joint.mJObj);
		sPos0.z      = HSD_JObjGetTranslationZ(joint.mJObj);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		sPos1.x = HSD_JObjGetTranslationX(joint.mJObj);
		sPos1.y = HSD_JObjGetTranslationY(joint.mJObj);
		sPos1.z = HSD_JObjGetTranslationZ(joint.mJObj);
		self->_990++;
		break;
	}
	case 2: {
		sTime += 0.16f;
		if (sTime >= 1.0f) {
			self->_990++;
		}
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		break;
	}
	case 3: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		sTime -= 0.16f;
		if (sTime <= 0.0f) {
			self->_990   = 0;
			s32 path2[4] = { 0, 2, 0, 1 };
			joint.Attach(GetJointFromRoot(self->mJObj, 4, path2));
			joint.SetTranslateX(sPos0.x, 0);
			joint.SetTranslateY(sPos0.y, 0);
			joint.SetTranslateZ(sPos0.z, 0);
			s32 path3[5] = { 0, 2, 0, 0, 0 };
			joint.Attach(GetJointFromRoot(self->mJObj, 5, path3));
			joint.SetTranslateX(sPos1.x, 0);
			joint.SetTranslateY(sPos1.y, 0);
			joint.SetTranslateZ(sPos1.z, 0);
			sTime = 0.0f;
		}
		break;
	}
	}
	if (self->mToyStatus.mFlags & 1) {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 6, path2));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 7, path3));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
	} else {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 6, path2));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 7, path3));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
	}
}
