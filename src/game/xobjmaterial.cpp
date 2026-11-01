#include "game/xobjs.h"
#include "game/hsd_dobj.h"
#include <math.h>
#include "game/evt.h"

extern "C" {
void HSD_JObjWalkTree(HSD_JObj* joint, HSD_JObjWalkTreeCallback callback, f32** arguments);
void HSD_JObjModifyFlags(HSD_JObj* joint, u32 flags, u32 mask);
void HSD_DObjModifyFlags(HSD_DObj* draw, u32 flags, u32 mask);
void HSD_MObjSetAlpha(HSD_MObj* material, f32 alpha);
}

struct ObjectArchiveLink {
	void* _00;
	CObjArchive* archive;
};

/**
 * @note Address: 0x800911C0
 * @note Size: 0xF4
 */
extern "C" void fn_800911C0(XObj* object)
{
	CJArchive* archive = &object->mAnim;
	archive->Release();
	if (object->mEvt._04 != NULL)
		fn_8007F040(&object->mEvt, TRUE, FALSE);
	if (object->mStatus.mFlags & 1) {
		object->CXObjIntp::Release();
		ObjectArchiveLink** slot = ((ObjectArchiveLink**)object->mAttr._00);
		if (slot != NULL && *slot != NULL) {
			ObjectArchiveLink* link = *slot;
			if (link->archive != NULL && link->archive->mRefCount <= 0)
				link->archive = NULL;
		}
	}
	object->mAttr._00 = NULL;
	object->mAttr._04 = NULL;
	object->mFloors   = NULL;
	object->mStatus.mFlags &= ~0x10001ULL;
}

/**
 * @note Address: 0x800912B4
 * @note Size: 0xB8
 */
extern "C" void fn_800912B4(HSD_JObj* joint, f32** arguments, s32 type)
{
	HSD_JObjModifyFlags(joint, 0x40000, 0x1C0000);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_DObjModifyFlags(draw, 2, 0xE);
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjClearFlags(material, 0x68000000);
			HSD_MObjSetFlags(material, 0x2000);
			HSD_MObjCompileTev(material);
		}
	}
}

/**
 * @note Address: 0x8009136C
 * @note Size: 0x100
 */
extern "C" void fn_8009136C(HSD_JObj* joint, f32** arguments, s32 type)
{
	CXObj* object = (CXObj*)arguments[0];
	f32 alpha     = *arguments[1];
	HSD_JObjModifyFlags(joint, 0x80000, 0x1C0000);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_DObjModifyFlags(draw, 4, 0xE);
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetFlags(material, 0x60000000);
			HSD_MObjSetFlags(material, 0x2000);
			HSD_MObjCompileTev(material);
			if (material->mat != NULL)
				HSD_MObjSetAlpha(material, alpha * object->GetMObjAlpha(material));
		}
	}
}

/**
 * @note Address: 0x8009146C
 * @note Size: 0x60
 */
extern "C" void fn_8009146C(CXObj* object)
{
	void* arguments[1] = { NULL };
	arguments[0]       = object;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_800912B4, (f32**)arguments);
	HSD_JObjModifyFlags(object->mJObj, 0x10000000, 0x70000000);
}

/**
 * @note Address: 0x800914CC
 * @note Size: 0x74
 */
extern "C" void fn_800914CC(CXObj* object, f32 alpha)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = object;
	arguments[1]       = &alpha;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_8009136C, (f32**)arguments);
	HSD_JObjModifyFlags(object->mJObj, 0x20000000, 0x70000000);
}

/**
 * @note Address: 0x80091540
 * @note Size: 0x1EC
 */
extern "C" void fn_80091540(HSD_JObj* joint, f32** arguments, s32 type)
{
	f32 factor = *arguments[1];
	HSD_JObjSetFlags(joint, 0x80);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetFlags(material, 4);
			if (material->mat != NULL && __fpclassifyf(factor) != FP_NAN) {
				material->mat->diffuse.r = (u8)(material->mat->diffuse.r * factor);
				material->mat->diffuse.g = (u8)(material->mat->diffuse.g * factor);
				material->mat->diffuse.b = (u8)(material->mat->diffuse.b * factor);
			}
			for (HSD_TObj* texture = HSD_MObjGetTObj(material); texture != NULL; texture = HSD_TObjGetNext(texture)) {
				texture->flags &= ~0x1F0;
				texture->flags |= 0x50;
				texture->flags &= ~0xF0000;
				texture->flags |= 0x70000;
			}
			HSD_MObjCompileTev(material);
		}
	}
}

/**
 * @note Address: 0x8009172C
 * @note Size: 0x58
 */
extern "C" void fn_8009172C(CXObj* object, f32 factor)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = object;
	arguments[1]       = &factor;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091540, (f32**)arguments);
}

/**
 * @note Address: 0x80091784
 * @note Size: 0xB0
 */
extern "C" void fn_80091784(HSD_JObj* joint, f32** arguments, s32 type)
{
	BOOL enabled = (BOOL)arguments[0];
	u8 alpha     = (u32)arguments[1];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL && material->pe != NULL) {
			if (enabled)
				material->pe->flags |= 6;
			else
				material->pe->flags &= ~6;
			material->pe->dst_alpha = alpha;
		}
	}
}

/**
 * @note Address: 0x80091834
 * @note Size: 0x54
 */
extern "C" void fn_80091834(CXObj* object, BOOL enabled, u32 value)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = (void*)enabled;
	arguments[1]       = (void*)(u8)value;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091784, (f32**)arguments);
}
