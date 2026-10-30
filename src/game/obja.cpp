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

// reads the anim archive at path, call until it returns true
// NONMATCHING: the dol's CFile ctor stores mArchive before mEntrynum, game/cfile.h has them the other way
BOOL fn_8008A38C(CJAnim* anim, const char* path)
{
	if (anim->mReading == 0) {
		anim->Release();
		if (path == NULL || *path == '\0') {
			return TRUE;
		}
		CFile file;
		if (file.ConvertPathToEntrynum(path) < 0) {
			return TRUE;
		}
		CObjArchive* archive = fn_80132620(GAME_ARCHIVES);
		if (archive != NULL) {
			anim->ReadAsync(archive, path, 0, 0, 0);
			anim->mReading++;
		} else {
			return TRUE;
		}
	} else if (anim->ReadCheck()) {
		anim->SetupScene(0, 0);
		anim->mReading = 0;
	}
	return anim->mReading == 0;
}

void fn_8008A4FC(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mReading = 0;
}

BOOL fn_8008A530(XObj* obj, s32 unused, s32 id)
{
	if (obj->mFlags & 0x0000040000000000ULL) {
		if (id == 0x38) {
			return FALSE;
		}
		s32 pos = obj->GetSoundPos();
		if (!fn_8009FDE4(GAME_CHR)) {
			fn_8007EEA4(GAME_SOUND, pos, obj, id, -1);
		}
		return TRUE;
	}
	return FALSE;
}

void fn_8008A604(XObj* obj, s32 unused, s16 id)
{
	if (obj->mFlags & 0x0000080000000000ULL) {
		s32 pos = obj->GetSoundPos();
		if (!fn_8009FDE4(GAME_CHR)) {
			fn_8007EEA4(GAME_SOUND, pos, obj, -1, id + 3000);
		}
	}
}

// a sound from where the object is, unless the robot talks over it
static inline void PlaySound(XObj* obj, s16 id)
{
	s32 pos = obj->GetSoundPos();
	if (!fn_8009FDE4(GAME_CHR)) {
		fn_8007EEA4(GAME_SOUND, pos, obj, -1, id);
	}
}

void fn_8008A6C0(XObj* obj, f32 speed)
{
	if (obj->mFlags & 0x800) {
		if (speed >= fn_8006EBC8(0x1F) / 100.0f) {
			PlaySound(obj, 2001);
		} else {
			PlaySound(obj, 2000);
		}
	}
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

// moves to x z but keeps inside the box around the anchor, true when it had to stop
BOOL fn_8008ADA0(XObj* obj, const Vec* dir, u8 axes, f32 x, f32 z)
{
	BOOL stopped = FALSE;
	if (fpclassify(obj->_898) == FP_NAN || fpclassify(obj->_89C) == FP_NAN) {
		obj->SetTranslateX(x, 0);
		obj->SetTranslateZ(z, 0);
	} else if (obj->_898 == x && obj->_89C == z) {
		obj->SetTranslateX(x, 0);
		obj->SetTranslateZ(z, 0);
	} else {
		Mtx m;
		Vec v;
		PSMTXRotRad(m, 'y', -obj->mMoveRotate.y);
		v.x = obj->_898 - x;
		v.z = obj->_89C - z;
		v.y = 0.0f;
		PSMTXMultVec(m, &v, &v);
		if (axes & 1) {
			if (dir->x > 0.0f && v.x < obj->mMinX) {
				v.x     = obj->mMinX;
				stopped = TRUE;
			} else if (dir->x < 0.0f && v.x > obj->mMaxX) {
				v.x     = obj->mMaxX;
				stopped = TRUE;
			}
		}
		if (axes & 2) {
			if (dir->z > 0.0f && v.z < obj->mMinZ) {
				v.z     = obj->mMinZ;
				stopped = TRUE;
			} else if (dir->z < 0.0f && v.z > obj->mMaxZ) {
				v.z     = obj->mMaxZ;
				stopped = TRUE;
			}
		}
		PSMTXRotRad(m, 'y', obj->mMoveRotate.y);
		PSMTXMultVec(m, &v, &v);
		obj->SetTranslateX(obj->_898 - v.x, 0);
		obj->SetTranslateZ(obj->_89C - v.z, 0);
	}
	return stopped;
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

BOOL fn_8008B224(const XObj* obj, CCamera* cam)
{
	HSD_JObj* jobj = obj->mJObj;
	if (jobj != NULL) {
		Vec pos = GetJObjPos(jobj);
		f32 x, y, z;
		GXProject(pos.x, pos.y, pos.z, cam->mViewMtx, cam->mProjection, cam->mViewport, &x, &y, &z);
		if (z > 0.0f && z < 1.0f) {
			return FALSE;
		}
		return TRUE;
	}
	return TRUE;
}

// whether a cylinder around the object is all on screen, and how big it shows
BOOL fn_8008B338(XObj* obj, f32* width, f32* height, CCamera* cam, f32 radius, f32 top)
{
	f32 left, right, bottom, up, x, y, z;
	f32 a, c, s;
	cam        = cam != NULL ? cam : GAME_CAMERA;
	f32 angle  = cam->mAngle.y;
	MtxPtr mtx = cam->mViewMtx;
	f32* proj  = cam->mProjection;
	f32* vp    = cam->mViewport;
	a          = 4.712389f + angle;
	c          = cos(a);
	s          = sin(a);
	GXProject(obj->mMoveTranslate.x + radius * s, obj->mMoveTranslate.y, obj->mMoveTranslate.z + radius * c, mtx, proj, vp, &left, &y, &z);
	a = 1.5707964f + angle;
	c = cos(a);
	s = sin(a);
	GXProject(obj->mMoveTranslate.x + radius * s, obj->mMoveTranslate.y, obj->mMoveTranslate.z + radius * c, mtx, proj, vp, &right, &y, &z);
	a = 3.1415927f + angle;
	c = cos(a);
	s = sin(a);
	GXProject(obj->mMoveTranslate.x + radius * s, obj->mMoveTranslate.y, obj->mMoveTranslate.z + radius * c, mtx, proj, vp, &x, &bottom,
	          &z);
	c = cos(angle);
	s = sin(angle);
	GXProject(obj->mMoveTranslate.x + radius * s, obj->mMoveTranslate.y + top, obj->mMoveTranslate.z + radius * c, mtx, proj, vp, &x, &up,
	          &z);
	if (left >= 0.0f && right < 640.0f && bottom >= 0.0f && up < 448.0f && z > 0.0f && z < 1.0f) {
		if (width != NULL) {
			*width = Abs(right - left);
		}
		if (height != NULL) {
			*height = Abs(up - bottom);
		}
		return TRUE;
	}
	return FALSE;
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

} // extern "C"

CVec CVec::operator*(Vec v) const
{
	CVec product = *this;
	product.x *= v.x;
	product.y *= v.y;
	product.z *= v.z;
	return product;
}

extern "C" {

void fn_8008C7CC(XObj* obj)
{
	obj->mNearDist = NAN;
	for (int i = 0; obj->mPoints[i].mKind != -1; i++) {
		if (obj->mPointMasks[0] & (1 << i)) {
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
		obj->mPointMasks[1] = obj->mPointMasks[0];
		for (int i = 0; obj->mPoints[i].mKind != -1; i++) {
			if (obj->IsPointOn(i)) {
				obj->PointOff(i);
			}
		}
	}
}

CVec fn_800901E8(const Vec* src, const Vec* normal)
{
	CVec v;
	C_VECReflect(src, normal, &v);
	return v;
}

void fn_80090254(XObj* obj)
{
	obj->CXObj::ClearAnim();
	obj->mFlags &= ~0x0008000000000000ULL;
}

void fn_800902A4(XObj* obj, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate)
{
	if (obj->mFlags & 0x0020000000000000ULL) {
		blend = 1.0f;
	}
	if (obj->GetAnimSet(id) != 0) {
		s32 i     = id % 1000;
		jointAnim = jointAnim != NULL ? jointAnim : obj->mAnim.GetAnimJoint(i);
		matAnim   = matAnim != NULL ? matAnim : obj->mAnim.GetMatAnimJoint(i);
		shapeAnim = shapeAnim != NULL ? shapeAnim : obj->mAnim.GetShapeAnimJoint(i);
		obj->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
		obj->mFlags |= 0x0008000000000000ULL;
	} else {
		obj->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
		obj->mFlags &= ~0x0008000000000000ULL;
	}
}

void fn_8009047C(XObj* obj)
{
	if ((obj->mFlags & 0x800) && (obj->mFlags & 0x10000)) {
		HSD_JObjSetFlags(obj->mJObj, 8);
		Vec scale = obj->mMoveScale;
		obj->SetScale(scale.x, -scale.y, scale.z, 0);
		if (obj->mJObj != NULL) {
			HSD_JObjDispAll(obj->mJObj, NULL, 7, 0);
		}
		obj->SetScale(scale, 0);
	}
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

// plays the next queued anim once the one on the model ended, then draws
void fn_800908C0(XObj* obj, Mtx vmtx)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	if (aobj != NULL && (aobj->flags & 0x44000000) && obj->HasMsg()) {
		s32 id;
		u16 frame;
		s16 rate;
		obj->PopMsg(id, frame, rate);
		if (id & 0xFFFF0000) {
			s32 lo          = id & 0xFFFF;
			s32 hi          = id >> 16;
			void* jointAnim = obj->GetAnimJoint(lo);
			void* matAnim   = obj->GetMatAnimJoint(hi);
			void* shapeAnim = obj->GetShapeAnimJoint(lo);
			obj->SetAnim(0, TRUE, jointAnim, matAnim, shapeAnim, frame, rate / 100.0f, 0.0f, 10.0f);
			if (lo >= 1000 || hi >= 1000) {
				obj->mFlags |= 0x0008000000000000ULL;
			}
		} else {
			obj->SetAnim(id, FALSE, NULL, NULL, NULL, frame, rate / 100.0f, 0.0f, 10.0f);
		}
		obj->UpdateAnim(1);
	}
	if (!(obj->mFlags & 0x800) && obj->mJObj != NULL) {
		HSD_JObjDispAll(obj->mJObj, vmtx, 7, 0);
	}
}

void fn_80090C84(XObj* obj, u32 frames)
{
	if (obj->mJObj != NULL) {
		for (int i = 0; i < 2; i++) {
			STEP_CHANNEL(obj->mColors[i].mColor.x, obj->mColors[i].mTarget.x, obj->mColors[i].mSpeed.x, frames);
			STEP_CHANNEL(obj->mColors[i].mColor.y, obj->mColors[i].mTarget.y, obj->mColors[i].mSpeed.y, frames);
			STEP_CHANNEL(obj->mColors[i].mColor.z, obj->mColors[i].mTarget.z, obj->mColors[i].mSpeed.z, frames);
		}
		obj->CXObj::mColors[1]
		    = (u32)obj->mColors[0].mColor.x << 24 | (u32)obj->mColors[0].mColor.y << 16 | (u32)obj->mColors[0].mColor.z << 8;
		obj->CXObj::mColors[0]
		    = (u32)obj->mColors[1].mColor.x << 24 | (u32)obj->mColors[1].mColor.y << 16 | (u32)obj->mColors[1].mColor.z << 8;
	}
}

void fn_80090ED4(XObj* obj, u32 frames)
{
	if (obj->mJObj != NULL) {
		obj->vfA8();
		if (obj->mFlags & 0x1000000000000000ULL) {
			if (obj->mMoveTranslate.y + obj->_798 > obj->_790) {
				obj->mMoveTranslate.mTarget.y = obj->_790;
				f32 speed                     = 0.15f + obj->mMoveTranslate.mSpeed.y;
				if (speed > 10.0f) {
					speed = 10.0f;
				}
				obj->mMoveTranslate.mSpeed.y = speed;
			} else {
				obj->mMoveTranslate.mSpeed.y = 0.0f;
				obj->mMoveTranslate.x += obj->_794;
				obj->mMoveTranslate.z += obj->_79C;
				obj->mMoveTranslate.y = obj->_790;
				obj->_794             = 0.0f;
				obj->_798             = 0.0f;
				obj->_79C             = 0.0f;
				obj->SetRotationX(0.0f, 0);
				obj->SetRotationZ(0.0f, 0);
				obj->mFlags &= ~0x1000000000000000ULL;
				if (obj->GetId() == 0x15) {
					obj->mFlags |= 0x2000000000000000ULL;
				} else if (obj->GetId() == 0x16 && (obj->mFlags & 0x4000000)) {
					obj->mFlags |= 0x2000000000000000ULL;
				}
			}
		}
		obj->CXObjIntp::StepRotate(frames);
		obj->CXObjIntp::StepTranslate(frames);
		if (obj->CXObjIntp::StepScale(frames)) {
			fn_8008C8A4(obj);
			fn_8008D220(obj);
		}
		obj->UpdateAnim(frames);
		HSD_JObjSetupMatrix(obj->mJObj);
	}
	if ((obj->mFlags & 0x0010000000000000ULL) || obj->mMoveTranslate.y != obj->_268) {
		fn_8008C8A4(obj);
	}
	if (obj->mEvt.mEvt != NULL) {
		CEvt_Update(&obj->mEvt, frames);
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
