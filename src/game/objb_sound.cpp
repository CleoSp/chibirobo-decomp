#include "game/objb.h"

// XObjSound, an object that plays sounds along with its anim

extern "C" {
s32 fn_8001DF00(void* sound, Vec* pos, s32 mode); // the pan of a point
}

extern "C" u8 lbl_80228FF8[];

// what plays the sounds, a table of them by anim and frame
class XObjSoundTableBase {
public:
	void* mTable; // _00
	u8 _04[0x8];  // _04
	// _0C vtable

	virtual void vf08();                             // _08
	virtual void Play(s32 anim, s32 pan, f32 frame); // _0C
};

class XObjSoundTable : public XObjSoundTableBase {
public:
	s32 _10; // _10
	s32 _14; // _14
};

// __vt__9XObjSound
class XObjSound : public XObj {
public:
	XObjSoundTable mTable; // _980

	virtual void PlayFrame(f32 frame); // _148
};

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800DD38C
 * @note Size: 0xB0
 */
void fn_800DD38C(XObjSound* obj, f32 frame)
{
	if ((obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
		obj->mTable.Play(obj->mAnimId, fn_8001DF00(GAME_SOUND, &obj->mMoveTranslate, 0), frame);
	}
}

/**
 * @note Address: 0x800DD43C
 * @note Size: 0xC8
 */
void fn_800DD43C(XObjSound* obj, u32 frames)
{
	fn_80090ED4(obj, frames);
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	obj->PlayFrame(aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f);
}

/**
 * @note Address: 0x800DD504
 * @note Size: 0x68
 */
void fn_800DD504(XObjSound* obj)
{
	fn_800928E8(obj);
	obj->mTable._10 = 0;
	obj->mTable._14 = 0;
	switch (obj->GetObjId()) {
	case 0x1D2:
		obj->mTable.mTable = lbl_80228FF8;
		break;
	}
}

/**
 * @note Address: 0x800DD56C
 * @note Size: 0x34
 */
void fn_800DD56C(XObjSound* obj, u32 id)
{
	fn_800929C4(obj, id);
	obj->mTable.mTable = NULL;
}

} // extern "C"
