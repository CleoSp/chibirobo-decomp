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
void fn_80091540(HSD_JObj* jobj, void* args, u32 type);

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
}
