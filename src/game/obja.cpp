#include "game/xobj.h"
#include "game/cjobjset.h"
#include "game/cgame.h"
#include "game/hsd_dobj.h"
#include "game/globals.h"
extern "C" {
#include <sysdolphin/class_alloc.h>
}

// XObj, the CXObjIntp every placed object in the house is built on

extern "C" {
void C_VECReflect(const Vec* source, const Vec* normal, Vec* result);
}

#if defined(VERSION_PAL)
#define ANIMATION_RATE(rate) ((rate) * (60.0f / (f32)lbl_80660D4C))
#else
#define ANIMATION_RATE(rate) (rate)
#endif

// the pe presets sit 0x5C into this unit's rodata, which isn't split out yet
struct XObjPEPresets {
	u8 _00[0x5C];          // _00
	HSD_PEDesc mReset;     // _5C
	HSD_PEDesc mPreset[3]; // _68
};

extern "C" {
extern const XObjPEPresets lbl_802091E8;
}

static inline u32 ShadowSize(XObj* object)
{
	return object->mAttr._00 != NULL ? object->mAttr._00->_0C : 0;
}

static inline void StepColor(f32& current, f32& target, f32& speed, u32 frames)
{
	if (current != target) {
		current += speed * frames;
		if (speed > 0.0f && current >= target) {
			current = target;
			speed   = 0.0f;
		}
		if (speed < 0.0f && current <= target) {
			current = target;
			speed   = 0.0f;
		}
	}
}

/**
 * @note Address: 0x8008DEC0
 * @note Size: 0x74
 */
extern "C" void fn_8008DEC0(XObj* object)
{
	if (object->mStatus.mFlags & 0x0200000000000000ULL) {
		object->_250[1] = object->_250[0];
		for (int i = 0; object->mFloors[i].mId != -1; i++) {
			if (object->IsFloorOn(i)) {
				object->ClearFloor(i);
			}
		}
	}
}

/**
 * @note Address: 0x800901E8
 * @note Size: 0x6C
 */
extern "C" CVec fn_800901E8(const Vec* source, const Vec* normal)
{
	CVec result;
	C_VECReflect(source, normal, &result);
	return result;
}

/**
 * @note Address: 0x80090254
 * @note Size: 0x50
 */
extern "C" void fn_80090254(XObj* object)
{
	object->CXObj::ClearAnim();
	object->mStatus.mFlags &= ~0x0008000000000000ULL;
}

/**
 * @note Address: 0x800902A4
 * @note Size: 0x1D8
 */
extern "C" void fn_800902A4(XObj* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                            f32 blend, f32 blendRate)
{
	if (object->mStatus.mFlags & 0x0020000000000000ULL)
		blend = 1.0f;
	if (object->GetAnimSet(id) != 0) {
		s32 index = id % 1000;
		jointAnim = jointAnim != NULL ? jointAnim : object->mAnim.GetAnimJoint(index);
		matAnim   = matAnim != NULL ? matAnim : object->mAnim.GetMatAnimJoint(index);
		shapeAnim = shapeAnim != NULL ? shapeAnim : object->mAnim.GetShapeAnimJoint(index);
		object->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, ANIMATION_RATE(rate), blend, blendRate);
		object->mStatus.mFlags |= 0x0008000000000000ULL;
	} else {
		object->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, ANIMATION_RATE(rate), blend, blendRate);
		object->mStatus.mFlags &= ~0x0008000000000000ULL;
	}
}

/**
 * @note Address: 0x8009047C
 * @note Size: 0xEC
 */
extern "C" void fn_8009047C(XObj* object)
{
	if ((object->mStatus.mFlags & 0x800ULL) && (object->mStatus.mFlags & 0x10000ULL)) {
		HSD_JObjSetFlags(object->mJObj, CLASSICAL_SCALE);
		Vec scale = object->mMoveScale;
		object->SetScale(scale.x, -scale.y, scale.z, 0);
		if (object->mJObj != NULL)
			HSD_JObjDispAll(object->mJObj, NULL, 7, 0);
		object->SetScale(scale, 0);
	}
}

/**
 * @note Address: 0x80090568
 * @note Size: 0x4
 */
extern "C" void fn_80090568()
{
}

/**
 * @note Address: 0x8009056C
 * @note Size: 0x2F8
 */
extern "C" void fn_8009056C(XObj* object, const Vec* origin, f32 opacity)
{
	if (!(object->mStatus.mFlags & 8ULL))
		return;
	if (ShadowSize(object) == 0)
		return;
	Vec position = *origin;
	Vec rotation = { -HALF_PI, 0.0f, 0.0f };
	Vec size     = object->mMoveScale * (f32)ShadowSize(object);
	fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
	u32 edge   = (u32)(128.0f * opacity);
	u32 center = (u32)(160.0f * opacity);
	fn_80198258(&lbl_8025C400.mGs, 12, center, edge, position.x, position.y, position.z, size.x, size.z, rotation.x, rotation.y,
	            rotation.z);
	PrimVertex vertices[26];
	f32 halfWidth      = size.x * 0.5f;
	f32 halfHeight     = size.z * 0.5f;
	PrimVertex* vertex = vertices;
	for (s32 i = 0; i < 13; ++i, vertex += 2) {
		f32 angle       = (PI / 6.0f) * i;
		vertex[0].pos.x = __fnmsubs(halfWidth, (f32)sin(angle), position.x);
		vertex[0].pos.z = __fmadds(halfHeight, (f32)cos(angle), position.z);
		vertex[0].pos.y = position.y;
		vertex[0].color = edge;
		// retail reads the scale again here
		f32 sine        = sin(angle);
		f32 outerWidth  = __fmadds(1.5f, object->mMoveScale.x, halfWidth);
		vertex[1].pos.x = __fnmsubs(outerWidth, sine, position.x);
		f32 cosine      = cos(angle);
		f32 outerHeight = __fmadds(1.5f, object->mMoveScale.z, halfHeight);
		vertex[1].pos.z = __fmadds(outerHeight, cosine, position.z);
		vertex[1].pos.y = position.y;
		vertex[1].color = 0;
	}
	fn_801988EC(&lbl_8025C400.mGs, GX_TRIANGLESTRIP, lbl_8025C400.mGs.mCamera->mViewMtx, vertices, 26);
}

/**
 * @note Address: 0x80090864
 * @note Size: 0x5C
 */
extern "C" void fn_80090864(XObj* object)
{
	if (object->mStatus.mFlags & 8ULL) {
		object->vfBC(&object->mMoveTranslate, 1.0f);
	}
}

/**
 * @note Address: 0x800908C0
 * @note Size: 0x3C4
 */
extern "C" void fn_800908C0(XObj* object, Mtx view)
{
	HSD_AObj* animation = GetAObj(object->mJObj);
	if (animation != NULL && (animation->flags & (AOBJ_NO_ANIM | AOBJ_REWINDED)) && object->HasQueuedAnim()) {
		s32 id;
		u16 frame;
		s16 rate;
		object->PopAnim(id, frame, rate);
		if (id & 0xFFFF0000) {
			s32 jointId             = id & 0xFFFF;
			s32 materialId          = id >> 16;
			void* jointAnimation    = object->vfF0(jointId);
			void* materialAnimation = object->vfF4(materialId);
			void* shapeAnimation    = object->vfF8(jointId);
			object->SetAnim(0, TRUE, jointAnimation, materialAnimation, shapeAnimation, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
			if (jointId >= 1000 || materialId >= 1000)
				object->mStatus.mFlags |= 0x0008000000000000ULL;
		} else {
			object->SetAnim(id, FALSE, NULL, NULL, NULL, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
		}
		object->UpdateAnim(1);
	}
	if (!(object->mStatus.mFlags & 0x800ULL)) {
		if (object->mJObj != NULL)
			HSD_JObjDispAll(object->mJObj, view, 7, 0);
	}
}

/**
 * @note Address: 0x80090C84
 * @note Size: 0x250
 */
extern "C" void fn_80090C84(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		for (s32 i = 0; i < 2; ++i) {
			XObjColorStep& color = object->mColorStep[i];
			StepColor(color.mValue.x, color.mTarget.x, color.mSpeed.x, frames);
			StepColor(color.mValue.y, color.mTarget.y, color.mSpeed.y, frames);
			StepColor(color.mValue.z, color.mTarget.z, color.mSpeed.z, frames);
		}
		object->mColors[1] = ((u32)object->mColorStep[0].mValue.x << 24) | ((u32)object->mColorStep[0].mValue.y << 16)
		                   | ((u32)object->mColorStep[0].mValue.z << 8);
		object->mColors[0] = ((u32)object->mColorStep[1].mValue.x << 24) | ((u32)object->mColorStep[1].mValue.y << 16)
		                   | ((u32)object->mColorStep[1].mValue.z << 8);
	}
}

/**
 * @note Address: 0x80090ED4
 * @note Size: 0x2EC
 */
extern "C" void fn_80090ED4(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		object->vfA8(frames);
		if (object->mStatus.mFlags & 0x1000000000000000ULL) {
			if (object->mMoveTranslate.y + object->_794.y > object->_790) {
				object->mMoveTranslate.mTarget.y = object->_790;
				f32 speed                        = 0.15f + object->mMoveTranslate.mSpeed.y;
				if (speed > 10.0f)
					speed = 10.0f;
				object->mMoveTranslate.mSpeed.y = speed;
			} else {
				object->mMoveTranslate.mSpeed.y = 0.0f;
				object->mMoveTranslate.x += object->_794.x;
				object->mMoveTranslate.z += object->_794.z;
				object->mMoveTranslate.y = object->_790;
				object->_794.x           = 0.0f;
				object->_794.y           = 0.0f;
				object->_794.z           = 0.0f;
				object->SetRotationX(0.0f, 0);
				object->SetRotationZ(0.0f, 0);
				object->mStatus.mFlags &= ~0x1000000000000000ULL;
				if ((s16)object->GetPool() == 21) {
					object->mStatus.mFlags |= 0x2000000000000000ULL;
				} else if ((s16)object->GetPool() == 22) {
					if (object->mStatus.mFlags & 0x04000000ULL)
						object->mStatus.mFlags |= 0x2000000000000000ULL;
				}
			}
		}
		object->CXObjIntp::StepRotate(frames);
		object->CXObjIntp::StepTranslate(frames);
		if (object->CXObjIntp::StepScale(frames)) {
			fn_8008C8A4((XObj*)object);
			fn_8008D220((XObj*)object);
		}
		object->UpdateAnim(frames);
		HSD_JObjSetupMatrix(object->mJObj);
	}
	if ((object->mStatus.mFlags & 0x0010000000000000ULL) || object->mMoveTranslate.y != object->_264.y) {
		fn_8008C8A4((XObj*)object);
	}
	if (object->mEvt.mData.mScript != NULL)
		CEvt_Update(&object->mEvt, frames);
}

/**
 * @note Address: 0x800911C0
 * @note Size: 0xF4
 */
extern "C" void fn_800911C0(XObj* object)
{
	CJArchive* archive = &object->mAnim;
	archive->Release();
	if (object->mEvt.mData.mScript != NULL)
		fn_8007F040(&object->mEvt, TRUE, FALSE);
	if (object->mStatus.mFlags & 1) {
		object->CXObjIntp::Release();
		XObjSource** slot = (XObjSource**)object->mAttr._00;
		if (slot != NULL && *slot != NULL) {
			XObjSource* source = *slot;
			if (source->mArchive != NULL && source->mArchive->mRefCount <= 0)
				source->mArchive = NULL;
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
	// NONMATCHING: the flags are read again between the stores
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
extern "C" void fn_80091834(CXObj* object, BOOL enabled, u8 value)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = (void*)enabled;
	arguments[1]       = (void*)value;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091784, (f32**)arguments);
}

// joint walk callback for XObj::vfD8, the args are five on/off flags
/**
 * @note Address: 0x80091888
 * @note Size: 0x3BC
 */
extern "C" void fn_80091888(HSD_JObj* joint, f32** arguments, s32 type)
{
	const XObjPEPresets* presets = &lbl_802091E8;
	u32 reset                    = (u32)arguments[0];
	u32 firstPreset              = (u32)arguments[1];
	u32 secondPreset             = (u32)arguments[2];
	u32 thirdPreset              = (u32)arguments[3];
	u32 toon                     = (u32)arguments[4];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			if (reset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mReset;
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
				if (material->rendermode & RENDER_NO_ZUPDATE)
					material->pe->flags &= ~0x20;
				if (material->rendermode & RENDER_ZMODE_ALWAYS)
					material->pe->flags &= ~0x10;
			}
			if (firstPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[0];
				*material->pe       = settings;
			} else if (secondPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[1];
				*material->pe       = settings;
			} else if (thirdPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[2];
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
			}
			if (toon) {
				if (!(fn_801B8E5C(material) & RENDER_TOON)) {
					HSD_MObjSetFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			} else {
				if (fn_801B8E5C(material) & RENDER_TOON) {
					HSD_MObjClearFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			}
		}
	}
}
