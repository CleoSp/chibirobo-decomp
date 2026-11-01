#include "game/hsd_jobj.h"
#include "game/cparticle.h"
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
#define GAME_EVT      (((CGameHigh*)(lbl_8025C400 + 0x40000))->mObjEvts)
#define GAME_SOUND    (lbl_8025C400 + 0x5D07C)
#define GAME_ARCHIVES ((CObjArchive*)(lbl_8025C400 + 0xF07C))
// the robot, which the objects keep quiet for while it is busy
#define GAME_CHR (*(void**)(lbl_8025C400 + 0x96BC))
extern "C" {
void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);
void fn_8007F194(XObjEvt* evt, void* list, XObj* obj);
void fn_8007F040(XObjEvt* evt, s32 a, s32 b);
void fn_80091540(HSD_JObj* jobj, void* args, u32 type);
void fn_80091888(HSD_JObj* jobj, void* args, u32 type);
BOOL fn_8009FDE4(void* chr);
void fn_8007EEA4(void* sound, s32 pos, XObj* obj, s16 a, s16 b);
void C_VECReflect(const Vec* src, const Vec* normal, Vec* dst);
CObjArchive* fn_80132620(CObjArchive* archives);
s32 fn_8006EBC8(s32 id);
void fn_8008C8A4(XObj* obj);
void fn_8008D220(XObj* obj);
void CEvt_Update(XObjEvt* evt, u32 frames);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
}

typedef HSD_JObjWalkTreeCallback WalkFunc;

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
		HSD_JObjWalkTree(jobj, (WalkFunc)JObjSetPObjFlags, (f32**)flags);
	}
}

static inline void ClearPObjFlags(HSD_JObj* jobj, u32 flags)
{
	if (jobj != NULL) {
		HSD_JObjWalkTree(jobj, (WalkFunc)JObjClearPObjFlags, (f32**)flags);
	}
}
extern "C" {

// a sound from where the object is, unless the robot talks over it
static inline void PlaySound(XObj* obj, s16 id)
{
	s32 pos = obj->GetSoundPos();
	if (!fn_8009FDE4(GAME_CHR)) {
		fn_8007EEA4(GAME_SOUND, pos, obj, -1, id);
	}
}
}
extern "C" {

// steps one channel of a color towards its target, stopping there
#define STEP_CHANNEL(c, t, v, frames) \
	if (c != t) {                     \
		c += v * frames;              \
		if (v > 0.0f && c >= t) {     \
			c = t;                    \
			v = 0.0f;                 \
		}                             \
		if (v < 0.0f && c <= t) {     \
			c = t;                    \
			v = 0.0f;                 \
		}                             \
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
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_800912B4, (f32**)&args);
	}
	HSD_JObjModifyFlags(obj->mJObj, 0x10000000, 0x70000000);
}

void fn_800914CC(XObj* obj, f32 alpha)
{
	XObjAlphaArgs args = { obj, &alpha };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_8009136C, (f32**)&args);
	}
	HSD_JObjModifyFlags(obj->mJObj, 0x20000000, 0x70000000);
}

void fn_80091540(HSD_JObj* jobj, void* args, u32 type)
{
	f32 scale = *((XObjAlphaArgs*)args)->mAlpha;
	HSD_JObjSetFlags(jobj, 0x80);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			HSD_MObjSetFlags(mobj, 4);
			if (mobj->mat != NULL && fpclassify(scale) != FP_NAN) {
				mobj->mat->diffuse.r = mobj->mat->diffuse.r * scale;
				mobj->mat->diffuse.g = mobj->mat->diffuse.g * scale;
				mobj->mat->diffuse.b = mobj->mat->diffuse.b * scale;
			}
			for (HSD_TObj* tobj = HSD_MObjGetTObj(mobj); tobj != NULL; tobj = HSD_TObjGetNext(tobj)) {
				tobj->flags = (tobj->flags &= ~0x1F0) | 0x50;
				tobj->flags = (tobj->flags &= ~0xF0000) | 0x70000;
			}
			HSD_MObjCompileTev(mobj);
		}
	}
}

void fn_8009172C(XObj* obj, f32 alpha)
{
	XObjAlphaArgs args = { obj, &alpha };
	if (obj->mJObj != NULL) {
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_80091540, (f32**)&args);
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
		HSD_JObjWalkTree(obj->mJObj, (WalkFunc)fn_80091784, (f32**)&args);
	}
}
}
