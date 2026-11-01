#ifndef _GAME_OBJB_LETICKER_H
#define _GAME_OBJB_LETICKER_H

#include "game/scr.h"
#include "game/cparticle.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
}
#include <sysdolphin/controller.h>
extern "C" {
#include <dolphin/os.h>
}

// the toys after XObjFree, game/objb_leticker.cpp and game/objb_army.cpp. class names from
// RTTI and vtable symbols, the rest is guessed. offsets are usa

// one sfx an anim plays between two of its frames, the table ends with an anim of -1
struct AnimSfxEntry {
	s32 mAnim;  // _00
	f32 mStart; // _04
	f32 mEnd;   // _08, below 0 for just the start frame
	s32 mType;  // _0C
	s32 mSfx;   // _10
	u8 _14;     // _14
	u8 mCount;  // _15
	u8 mStep;   // _16
	u8 _17;     // _17
};

// __vt__7AnimSfx, plays the sfx of an object's anims as the frames go by, game/sound.cpp
class AnimSfx {
public:
	AnimSfxEntry* mTable; // _00
	s32 mAnim;            // _04
	f32 mFrame;           // _08
	// _0C vtable

	virtual void vf08();                             // _08
	virtual void Play(s32 anim, s32 pan, f32 frame); // _0C
};

// one 0x20 byte entry of an object's point table, ends with a kind of -1
struct XObjPoint {
	s16 mKind;    // _00
	s16 _02;      // _02
	s32 mPos[3];  // _04, hundredths
	s32 mRotY;    // _10, degrees
	s32 mPos2[3]; // _14, hundredths
};

// one slot of an object's message ring, game/scr.h only has it as two words
struct XObjMsgSlot {
	s32 mId;    // _00
	u16 mFrame; // _04
	s16 mRate;  // _06, hundredths
};

// puts a message in the first free slot of the ring, game/scr.h has the frame and the rate
// as one word
static inline void PushMsg(XObj* obj, s32 id, u16 frame, s16 rate)
{
	for (int i = 0; i < 16; i++) {
		s32 j = (obj->mMsgCount + i) % 16;
		if (obj->mMsgs[j].mId == -1) {
			obj->mMsgs[j].mId                    = id;
			((XObjMsgSlot*)obj->mMsgs)[j].mFrame = frame;
			((XObjMsgSlot*)obj->mMsgs)[j].mRate  = rate;
			return;
		}
	}
}

extern "C" {
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXMultVecArray(const Mtx m, const Vec* src, Vec* dst, u32 count);
void fn_80198F1C(void* gs, Vec* pos, u32* color, u16 count); // draws a strip of points
void fn_8001E468(CSound* sound, s32 sfx, s32 pan, s32 arg);
void fn_8008D220(XObj* obj); // takes the points on
void fn_8008C8A4(XObj* obj);
void HSD_AudioSFXSetPitchByID(s32 sfx, s32 pitch);
}

// the joint at the end of a path of child and next steps, null when it runs out
static inline HSD_JObj* FindJoint(HSD_JObj* jobj, s32* path, s32 depth)
{
	s32* step = path;
	if (jobj == NULL) {
		return NULL;
	}
	for (s32 i = 0; i < depth; i++, step++) {
		jobj = HSD_JObjGetChild(jobj);
		for (s32 j = 0; j < *step; j++) {
			jobj = HSD_JObjGetNext(jobj);
			if (jobj == NULL) {
				return NULL;
			}
		}
	}
	return jobj;
}

// sets the object's point table, the points take effect at once
static inline void SetPoints(XObj* obj, XObjPoint* points)
{
	obj->mPoints       = points;
	obj->mPointMaskOld = 0;
	obj->mPointMask    = 0;
	fn_8008D220(obj);
	fn_8008C8A4(obj);
}

// the three render objects the game keeps at _290, an id count of 0 means not set
struct UnkRenderA {
	u8 _00[0xC]; // _00
	s32 _0C;     // _0C
	s32 _10;     // _10
	s32 _14;     // _14
	s32 _18;     // _18
};

struct UnkRenderB {
	u8 _00[0x74]; // _00
	s32 _74;      // _74
	s32 _78;      // _78
	s32 _7C;      // _7C
};

struct UnkRenderC {
	u8 _00[0x30]; // _00
	s32 _30;      // _30
};

// the game object's render state, game/objb.h has it with the rest of the game
struct UnkGameRender {
	u8 _00[0x290];    // _00
	UnkRenderA* _290; // _290
	UnkRenderB* _294; // _294
	UnkRenderC* _298; // _298
	s32 _29C;         // _29C
};

extern "C" {
void HSD_StateInvalidate(s32 state);
void fn_8019B56C(void* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_80198258(void* gs, u32 segments, u32 centerColor, u32 edgeColor, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz);
}

// the parts of the game object these reach
#define GAME_FLAGS    (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_PARTICLE ((CParticle*)(lbl_8025C400 + 0x9114))
#define GAME_PLAYER   (*(XObj**)(lbl_8025C400 + 0x96BC))
#define GAME_OBJS     (((UnkGameObjs*)lbl_8025C400)->mObjects)
#define GAME_OBJTABLE ((XObj**)(lbl_8025C400 + 0x9860))

// the game's object table
struct UnkGameObjs {
	u8 _00[0x9860];        // _00
	XObj* mObjects[0x400]; // _9860
	s32 mObjectCount;      // _A860
};

// the game's camera, game/cmotioncontrolcamera.h has it, its header has its own XObj
class CMotionControlCamera {
public:
	void PopAction();
};
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5D38))

class CCamera;

// game/ccameraaction.h has these, its header has its own XObj
class CInterfaceCameraAction {
public:
	CVec mEye;      // _00
	CVec mInterest; // _10
	CVec mAngle;    // _20
	f32 mDistance;  // _30
	f32 mFov;       // _34
	u32 _38;        // _38
	// _3C vtable

	virtual ~CInterfaceCameraAction() { }       // _08
	virtual void PreviousUpdate() { }           // _0C
	virtual void Update(u32 step) = 0;          // _10
	virtual void vf14() { }                     // _14
	virtual void SetCamera(CCamera* camera) { } // _18
	virtual void Init() { }                     // _1C
	virtual void vf20() { }                     // _20
	virtual void vf24() { }                     // _24
	virtual void vf28() { }                     // _28
	virtual void SetEye(f32 x, f32 y, f32 z)    // _2C
	{
		mEye.x = x;
		mEye.y = y;
		mEye.z = z;
	}
	virtual void SetInterest(f32 x, f32 y, f32 z) // _30
	{
		mInterest.x = x;
		mInterest.y = y;
		mInterest.z = z;
	}
	virtual void SetAngle(f32 x, f32 y, f32 z) // _34
	{
		mAngle.x = x;
		mAngle.y = y;
		mAngle.z = z;
	}
	virtual void SetEye(const Vec& eye) { SetEye(eye.x, eye.y, eye.z); }                               // _38
	virtual void SetInterest(const Vec& interest) { SetInterest(interest.x, interest.y, interest.z); } // _3C
	virtual void SetAngle(const Vec& angle) { SetAngle(angle.x, angle.y, angle.z); }                   // _40
	virtual void SetDistance(f32 distance) { mDistance = distance; }                                   // _44
	virtual void SetFov(f32 fov) { mFov = fov; }                                                       // _48
	virtual void SetAngleZ(f32 z) { mAngle.z = z; }                                                    // _4C
	virtual u32 vf50() { return 0; }                                                                   // _50
	virtual u32 Get_38() { return _38; }                                                               // _54
	virtual f32 vf58();                                                                                // _58
	virtual u32 vf5C() { return 0; }                                                                   // _5C
	virtual XObj* GetTarget() { return NULL; }                                                         // _60
};

// behind the player at an angle off its facing
class CCameraActionLadr : public CInterfaceCameraAction {
public:
	f32 mAngle; // _40, added to the player's y rotation

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual u32 Get_38() { return 11; } // _54
	virtual Vec GetCameraTarget();      // _64, the player

	void CalcEye(Vec* eye, f32 angle);
};

#endif
