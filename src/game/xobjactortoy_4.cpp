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
extern "C" void fn_8011A050(XObjActorToy* self, u32 frames);
extern "C" void fn_80119774(XObjActorToy* self, u32 frames);

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

void XObjActorToy::vfAC()
{
	fn_80107620(this);
	if (mPart0.mFlags & 0x10000) {
		mPart0.CJObj::SetTranslate(ToyTranslationX(*this), ToyTranslationY(*this), ToyTranslationZ(*this), 1);
		mPart0.CJObj::SetScale(ToyScaleX(*this), ToyScaleY(*this), ToyScaleZ(*this), 1);
		mPart0.CJObj::SetRotationY(ToyRotationY(*this), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mPart0.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mPart0.mJObj, 8);
		}
		mPart0.vfAC();
	}
	if (mPart1.mFlags & 0x10000) {
		mPart1.CJObj::SetTranslate(ToyTranslationX(*this), ToyTranslationY(*this), ToyTranslationZ(*this), 1);
		mPart1.CJObj::SetScale(ToyScaleX(*this), ToyScaleY(*this), ToyScaleZ(*this), 1);
		mPart1.CJObj::SetRotationY(ToyRotationY(*this), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mPart1.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mPart1.mJObj, 8);
		}
		mPart1.vfAC();
	}
}
extern "C" void fn_80119C24(XObjActorToy* self, u32 frames);
extern "C" void fn_8011A390(XObjActorToy* self, u32 frames);
