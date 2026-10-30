#include "game/obja.h"
#include "game/ccamera.h"
#include "game/gmath.h"
#include "game/hsd_dobj.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"
#include <math.h>

extern "C" u8 lbl_8025C400[];
#define GAME_CAMERA ((CCamera*)(lbl_8025C400 + 0xE5D38))
// the event list the objects register with. the game reaches it through a pointer to its
// second half, which keeps the two offsets apart
struct CGameHigh {
	u8 _00000[0x61708]; // _00000
	u8 mObjEvts[0x4];   // _61708
};
#define GAME_EVT (((CGameHigh*)(lbl_8025C400 + 0x40000))->mObjEvts)

extern "C" {
void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);
void fn_8007F194(XObjEvt* evt, void* list, XObj* obj);
void fn_8007F040(XObjEvt* evt, s32 a, s32 b);
void fn_80091540(HSD_JObj* jobj, void* args, u32 type);
void fn_80091888(HSD_JObj* jobj, void* args, u32 type);
}

typedef void (*WalkFunc)(HSD_JObj*, void*, u32);

// what the jobj walks get
struct XObjArgs {
	XObj* mObj; // _00
};

struct XObjAlphaArgs {
	XObj* mObj;  // _00
	f32* mAlpha; // _04
};

struct XObjCullArgs {
	s32 mOn;   // _00
	s32 mMode; // _04
};

static inline void SetPObjFlags(HSD_JObj* jobj, u32 flags)
{
	if (jobj != NULL) {
		HSD_JObjWalkTree(jobj, (WalkFunc)JObjSetPObjFlags, (void*)flags);
	}
}

static inline void ClearPObjFlags(HSD_JObj* jobj, u32 flags)
{
	if (jobj != NULL) {
		HSD_JObjWalkTree(jobj, (WalkFunc)JObjClearPObjFlags, (void*)flags);
	}
}

// XObj, the CXObjIntp every placed object in the house is built on. nothing here has a name
// in the dol, so everything is extern "C" under its address

// ---- functions ----

extern "C" {

void fn_8008A360(CJAnim* anim)
{
	if (anim->mArchive != NULL) {
		anim->CJArchive::Release();
	}
}

void fn_8008A4FC(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mReading = 0;
}

BOOL fn_8008A818(XObj* obj)
{
	return TRUE;
}

void fn_8008A820(XObj* obj, s32 i, u8 r, u8 g, u8 b, f32 seconds)
{
	obj->mColors[i].mTarget.x = r;
	obj->mColors[i].mTarget.y = g;
	obj->mColors[i].mTarget.z = b;
	if (seconds == 0.0f) {
		obj->mColors[i].mColor.x = r;
		obj->mColors[i].mColor.y = g;
		obj->mColors[i].mColor.z = b;
		obj->mColors[i].mSpeed.x = obj->mColors[i].mSpeed.y = obj->mColors[i].mSpeed.z = 0.0f;
	} else {
		obj->mColors[i].mSpeed.x = (r - obj->mColors[i].mColor.x) / (60.0f * seconds);
		obj->mColors[i].mSpeed.y = (g - obj->mColors[i].mColor.y) / (60.0f * seconds);
		obj->mColors[i].mSpeed.z = (b - obj->mColors[i].mColor.z) / (60.0f * seconds);
	}
}

void fn_8008B0D8(XObj* obj, s32 mode)
{
	switch (mode) {
	case 0:
		ClearPObjFlags(obj->mJObj, 0x4000);
		ClearPObjFlags(obj->mJObj, 0x8000);
		break;
	case 2:
		ClearPObjFlags(obj->mJObj, 0x4000);
		SetPObjFlags(obj->mJObj, 0x8000);
		break;
	case 1:
		SetPObjFlags(obj->mJObj, 0x4000);
		ClearPObjFlags(obj->mJObj, 0x8000);
		break;
	case 3:
		SetPObjFlags(obj->mJObj, 0x4000);
		SetPObjFlags(obj->mJObj, 0x8000);
		break;
	}
}

void fn_8008B5D4(XObj* obj, const Vec* offset, f32* sx, f32* sy, f32* sz, CCamera* cam)
{
	f32 x, y, z;
	cam = cam != NULL ? cam : GAME_CAMERA;
	GXProject(obj->mMoveTranslate.x + offset->x, obj->mMoveTranslate.y + offset->y, obj->mMoveTranslate.z + offset->z, cam->mViewMtx,
	          cam->mProjection, cam->mViewport, &x, &y, &z);
	if (sx != NULL) {
		*sx = x;
	}
	if (sy != NULL) {
		*sy = y;
	}
	if (sz != NULL) {
		*sz = z;
	}
}

void fn_8008C7CC(XObj* obj)
{
	obj->mNearDist = NAN;
	for (int i = 0; obj->mPoints[i].mKind != -1; i++) {
		if (obj->mPointMask & (1 << i)) {
			if (fpclassify(obj->mNearDist) == FP_NAN || obj->mPointDist[i] > obj->mNearDist) {
				obj->mNearPoint = i;
				obj->mNearDist  = obj->mPointDist[i];
			}
		}
	}
}

void fn_8008DEC0(XObj* obj)
{
	if (obj->mFlags & 0x0200000000000000ULL) {
		obj->mPointMaskOld = obj->mPointMask;
		for (int i = 0; obj->mPoints[i].mKind != -1; i++) {
			if (obj->IsPointOn(i)) {
				obj->PointOff(i);
			}
		}
	}
}

void fn_80090254(XObj* obj)
{
	obj->CXObj::ClearAnim();
	obj->mFlags &= ~0x0008000000000000ULL;
}

void fn_80090568(XObj* obj)
{
}

void fn_80090864(XObj* obj)
{
	if (obj->mFlags & 8) {
		obj->vfBC(&obj->mMoveTranslate, 1.0f);
	}
}

void fn_800911C0(XObj* obj)
{
	obj->mAnim.Release();
	if (obj->mEvt.mEvt != NULL) {
		fn_8007F040(&obj->mEvt, 1, 0);
	}
	if (obj->mFlags & 1) {
		obj->CXObjIntp::Release();
		if (obj->mInfo != NULL && obj->mInfo->mSource != NULL && obj->mInfo->mSource->mArchive != NULL
		    && obj->mInfo->mSource->mArchive->mRefCount <= 0) {
			obj->mInfo->mSource->mArchive = NULL;
		}
	}
	obj->mInfo   = NULL;
	obj->_244    = 0;
	obj->mPoints = NULL;
	obj->mFlags &= ~0x10001ULL;
}

void fn_800912B4(HSD_JObj* jobj, void* args, u32 type)
{
	HSD_JObjModifyFlags(jobj, 0x40000, 0x1C0000);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_DObjModifyFlags(dobj, 2, 0xE);
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			HSD_MObjClearFlags(mobj, 0x68000000);
			HSD_MObjSetFlags(mobj, 0x2000);
			HSD_MObjCompileTev(mobj);
		}
	}
}

void fn_8009136C(HSD_JObj* jobj, XObjAlphaArgs* args, u32 type)
{
	XObj* obj = args->mObj;
	f32 alpha = *args->mAlpha;
	HSD_JObjModifyFlags(jobj, 0x80000, 0x1C0000);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_DObjModifyFlags(dobj, 4, 0xE);
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			HSD_MObjSetFlags(mobj, 0x60000000);
			HSD_MObjSetFlags(mobj, 0x2000);
			HSD_MObjCompileTev(mobj);
			if (mobj->mat != NULL) {
				HSD_MObjSetAlpha(mobj, alpha * obj->GetMObjAlpha(mobj));
			}
		}
	}
}

void fn_8009146C(XObj* obj)
{
	XObjArgs args = { obj };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_800912B4, &args);
	}
	HSD_JObjModifyFlags(obj->mJObj, 0x10000000, 0x70000000);
}

void fn_800914CC(XObj* obj, f32 alpha)
{
	XObjAlphaArgs args = { obj, &alpha };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_8009136C, &args);
	}
	HSD_JObjModifyFlags(obj->mJObj, 0x20000000, 0x70000000);
}

void fn_8009172C(XObj* obj, f32 alpha)
{
	XObjAlphaArgs args = { obj, &alpha };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_80091540, &args);
	}
}

void fn_80091784(HSD_JObj* jobj, XObjCullArgs* args, u32 type)
{
	s32 on  = args->mOn;
	u8 mode = args->mMode;
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL && mobj->pe != NULL) {
			if (on) {
				mobj->pe->flags |= 6;
			} else {
				mobj->pe->flags &= ~6;
			}
			mobj->pe->dst_alpha = mode;
		}
	}
}

void fn_80091834(XObj* obj, s32 on, u8 mode)
{
	XObjCullArgs args = { on, mode };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_80091784, &args);
	}
}

void fn_80091C44(XObj* obj, void* args)
{
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_80091888, args);
	}
}

void fn_800928E8(XObj* obj)
{
	obj->_80C = 0;
	obj->_788 = -1;
	obj->_78C = 0;
	obj->_790 = 0.0f;
	obj->_794 = 0.0f;
	obj->_798 = 0.0f;
	obj->_79C = 0.0f;
	obj->ClearMsgs();
	obj->_898 = obj->_89C = NAN;
	obj->_8B0             = 0;
	obj->_8B0 |= 0x80000000;
	obj->mColors[0].mColor.x = obj->mColors[0].mColor.y = obj->mColors[0].mColor.z = 255.0f;
	obj->mColors[0].mTarget.x = obj->mColors[0].mTarget.y = obj->mColors[0].mTarget.z = 255.0f;
	obj->mColors[0].mSpeed.x = obj->mColors[0].mSpeed.y = obj->mColors[0].mSpeed.z = 0.0f;
	obj->mColors[1].mColor.x = obj->mColors[1].mColor.y = obj->mColors[1].mColor.z = 0.0f;
	obj->mColors[1].mTarget.x = obj->mColors[1].mTarget.y = obj->mColors[1].mTarget.z = 0.0f;
	obj->mColors[1].mSpeed.x = obj->mColors[1].mSpeed.y = obj->mColors[1].mSpeed.z = 0.0f;
}

void fn_800929C4(XObj* obj, u32 id)
{
	obj->CXObj::Init(id);
	obj->mFlags  = 0;
	obj->_238    = 0;
	obj->mInfo   = NULL;
	obj->_244    = 0;
	obj->mPoints = NULL;
	obj->_258    = 0;
	obj->Reset();
	fn_8007F194(&obj->mEvt, GAME_EVT, obj);
	obj->ClearMsgs();
	obj->mAnim.Clear();
}

} // extern "C"
