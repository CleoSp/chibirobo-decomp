#include "game/particle_srt.h"
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
	if (anim->mLoading == 0) {
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
			anim->mLoading++;
		} else {
			return TRUE;
		}
	} else if (anim->ReadCheck()) {
		anim->SetupScene(0, 0);
		anim->mLoading = 0;
	}
	return anim->mLoading == 0;
}

void fn_8008A4FC(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mLoading = 0;
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
	if (obj->mFlags & 0x0000080000000000ULL) {
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
void fn_80091540(HSD_JObj* jobj, void* args, u32 type);

} // extern "C"
