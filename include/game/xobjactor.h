#ifndef _GAME_XOBJACTOR_H
#define _GAME_XOBJACTOR_H

#include "game/xobj.h"
#include "game/ctpl.h"
#include "game/csound.h"
#include "game/cptcl.h"

// the people and toys, class names from RTTI and the vtables at 0x8022BF90 (XObjActor) and on.
// offsets are usa, everything past XObj sits 0x10 lower in japan

struct XObjActorPart {
	u32 _00;            // _00
	u32 _04;            // _04
	u8 _08[0x38 - 0x8]; // _08
};

// the base of the people and toys
class XObjActor : public XObj {
public:
	// _00-_980 = XObj
	f32 _980;         // _980
	f32 _984;         // _984, move speed
	f32 _988;         // _988
	f32 _98C;         // _98C
	s32 _990;         // _990, the kind's own step
	u32 _994;         // _994
	CStatus _998;     // _998, 2 a sound is due, 0x2000 busy
	u32 _9A0;         // _9A0
	s32 _9A4;         // _9A4
	f32 _9A8;         // _9A8
	f32 _9AC;         // _9AC
	Vec mHitPos;      // _9B0
	Vec _9BC;         // _9BC
	Vec _9C8;         // _9C8, velocity
	f32 mHitStrength; // _9D4
	f32 _9D8;         // _9D8
	f32 _9DC;         // _9DC
	s32 mCmd[20];     // _9E0, the event command vf124 got and its args, the rest is the kind's scratch
	u8 _A30[0xA38 - 0xA30];
	f32 _A38; // _A38
	f32 _A3C; // _A3C
	Vec _A40; // _A40
	s16 _A4C; // _A4C, id of the attached object, -1 for none
	u8 _A4E[0xA50 - 0xA4E];
	XObj* _A50; // _A50, the attached object
	u8 _A54[0xA84 - 0xA54];
	XObj* _A84;              // _A84, held object
	u32 _A88;                // _A88
	AnimSfx mAnimSfx;        // _A8C, its events are the kind's table
	s32 mLookMode;           // _A9C, 0 ahead, 1 at mLookTarget, 3 at mLookAngle
	f32 _AA0;                // _AA0
	f32 mLookAngle;          // _AA4
	f32 mLookRate;           // _AA8
	s32 mLookTarget;         // _AAC, a Vec*
	f32 _AB0;                // _AB0
	XObjActorPart mParts[8]; // _AB4
	u8 _C74[0xC78 - 0xC74];

	XObjActor();

	virtual ~XObjActor();               // _08
	virtual void vf148(u32 frames);     // _148, the kind's own update
	virtual void vf14C(u32 frames) = 0; // _14C, runs the subclass's mState
	virtual void vf150()           = 0; // _150, clears mState
	virtual void vf154(u32 frames);     // _154
	virtual void vf158() = 0;           // _158, works off the queued flags
	virtual void vf15C();               // _15C, resets the actor
	virtual void vf160();               // _160
	virtual void vf164(f32 frame);      // _164
	virtual HSD_JObj* vf168();          // _168, the head joint
	virtual CVec vf16C();               // _16C, the head joint's position
	virtual f32 vf170();                // _170, turn limits
	virtual f32 vf174();                // _174
	virtual f32 vf178();                // _178
	virtual f32 vf17C();                // _17C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActor) == 0xC68);
#else
STATIC_ASSERT(sizeof(XObjActor) == 0xC78);
#endif

// toys made of two more objects, read after the toy itself
class XObjActorToy : public XObjActor {
public:
	typedef void (XObjActorToy::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78, run every frame by vf14C
	CStatus _C84;     // _C84, kinds queued to hide
	s32 mLoad;        // _C8C, which part vf9C is reading
	XObj mFirst;      // _C90
	XObj mSecond;     // _1610, _1600 in japan
	s32 _1F90;        // _1F90, _1F70 in japan
	CVec _1F94;       // _1F94
	u8 _1FA4[0x2098 - 0x1FA4];
	CStatus _2098; // _2098, _2068 in japan

	XObjActorToy();

	virtual ~XObjActorToy();                  // _08
	virtual void Release();                   // _54
	virtual void vf90(u32 id);                // _90
	virtual void vf94();                      // _94
	virtual void vf98();                      // _98
	virtual BOOL vf9C(const CoordInfo& info); // _9C
	virtual void vfAC();                      // _AC
	virtual void vfC8();                      // _C8
	virtual s32 vf124(s32 count, s32* args);  // _124
	virtual void vf130();                     // _130
	virtual void vf148(u32 frames);           // _148
	virtual void vf14C(u32 frames);           // _14C
	virtual void vf150();                     // _150
	virtual void vf158();                     // _158
	virtual void vf15C();                     // _15C
	virtual void vf164(f32 frame);            // _164
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorToy) == 0x2070);
#else
STATIC_ASSERT(sizeof(XObjActorToy) == 0x20A0);
#endif

// jenny
class XObjActorJenny : public XObjActor {
public:
	typedef void (XObjActorJenny::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78
	f32 _C84;         // _C84
	f32 _C88;         // _C88
	u32 _C8C;         // _C8C
	CStatus _C90;     // _C90, 4 head joint, 8 joint shown
	CStatus _C98;     // _C98, kinds queued to hide
	u8 _CA0;          // _CA0
	u8 _CA1[0xCA8 - 0xCA1];

	XObjActorJenny();

	virtual ~XObjActorJenny();               // _08
	virtual void UpdateAnim(u32 frames);     // _88
	virtual void vfA4(u32 frames);           // _A4
	virtual s32 vf124(s32 count, s32* args); // _124
	virtual void vf148(u32 frames);          // _148
	virtual void vf14C(u32 frames);          // _14C
	virtual void vf150();                    // _150
	virtual void vf158();                    // _158
	virtual void vf15C();                    // _15C
	virtual HSD_JObj* vf168();               // _168
	virtual CVec vf16C();                    // _16C
	virtual f32 vf178();                     // _178
	virtual f32 vf17C();                     // _17C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorJenny) == 0xC98);
#else
STATIC_ASSERT(sizeof(XObjActorJenny) == 0xCA8);
#endif

// mama
class XObjActorMama : public XObjActor {
public:
	typedef void (XObjActorMama::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78
	f32 _C84;         // _C84
	u32 _C88;         // _C88
	CStatus _C8C;     // _C8C, kinds queued to hide
	u8 _C94[0xC98 - 0xC94];

	XObjActorMama();

	virtual ~XObjActorMama();                // _08
	virtual void vfA4(u32 frames);           // _A4
	virtual s32 vf124(s32 count, s32* args); // _124
	virtual void vf148(u32 frames);          // _148
	virtual void vf14C(u32 frames);          // _14C
	virtual void vf150();                    // _150
	virtual void vf158();                    // _158
	virtual void vf15C();                    // _15C
	virtual HSD_JObj* vf168();               // _168
	virtual CVec vf16C();                    // _16C
	virtual f32 vf170();                     // _170
	virtual f32 vf174();                     // _174
	virtual f32 vf178();                     // _178
	virtual f32 vf17C();                     // _17C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorMama) == 0xC88);
#else
STATIC_ASSERT(sizeof(XObjActorMama) == 0xC98);
#endif

// papa
class XObjActorPapa : public XObjActor {
public:
	typedef void (XObjActorPapa::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78
	f32 _C84;         // _C84
	u32 _C88;         // _C88
	CStatus _C8C;     // _C8C, kinds queued to hide
	u8 _C94;          // _C94
	u8 _C95[0xC98 - 0xC95];

	XObjActorPapa();

	virtual ~XObjActorPapa();                // _08
	virtual s32 vf124(s32 count, s32* args); // _124
	virtual void vf148(u32 frames);          // _148
	virtual void vf14C(u32 frames);          // _14C
	virtual void vf150();                    // _150
	virtual void vf158();                    // _158
	virtual void vf15C();                    // _15C
	virtual HSD_JObj* vf168();               // _168
	virtual CVec vf16C();                    // _16C
	virtual f32 vf178();                     // _178
	virtual f32 vf17C();                     // _17C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorPapa) == 0xC88);
#else
STATIC_ASSERT(sizeof(XObjActorPapa) == 0xC98);
#endif

// tao, the dog
class XObjActorTao : public XObjActor {
public:
	typedef void (XObjActorTao::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78
	f32 _C84;         // _C84
	u32 _C88;         // _C88
	CStatus _C8C;     // _C8C, kinds queued to hide
	u8 _C94[0xCF4 - 0xC94];
	s16 _CF4; // _CF4
	u8 _CF6[0xCF8 - 0xCF6];
	u32 _CF8; // _CF8
	u32 _CFC; // _CFC

	XObjActorTao();

	virtual ~XObjActorTao();                 // _08
	virtual s32 vf124(s32 count, s32* args); // _124
	virtual void vf148(u32 frames);          // _148
	virtual void vf14C(u32 frames);          // _14C
	virtual void vf150();                    // _150
	virtual void vf158();                    // _158
	virtual void vf15C();                    // _15C
	virtual HSD_JObj* vf168();               // _168
	virtual CVec vf16C();                    // _16C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorTao) == 0xCF0);
#else
STATIC_ASSERT(sizeof(XObjActorTao) == 0xD00);
#endif

// deca
class XObjActorDeca : public XObjActor {
public:
	typedef void (XObjActorDeca::*StateFunc)(u32 frames);

	// _00-_C78 = XObjActor
	StateFunc mState; // _C78
	f32 _C84;         // _C84
	f32 _C88;         // _C88
	u32 _C8C;         // _C8C
	CStatus _C90;     // _C90, 4 head joint
	CStatus _C98;     // _C98
	f32 _CA0;         // _CA0
	f32 _CA4;         // _CA4

	XObjActorDeca();

	virtual ~XObjActorDeca();                // _08
	virtual void UpdateAnim(u32 frames);     // _88
	virtual void vfA4(u32 frames);           // _A4
	virtual Vec GetCameraTarget();           // _EC
	virtual s32 vf124(s32 count, s32* args); // _124
	virtual void vf148(u32 frames);          // _148
	virtual void vf14C(u32 frames);          // _14C
	virtual void vf150();                    // _150
	virtual void vf158();                    // _158
	virtual void vf15C();                    // _15C
	virtual HSD_JObj* vf168();               // _168
	virtual CVec vf16C();                    // _16C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActorDeca) == 0xC98);
#else
STATIC_ASSERT(sizeof(XObjActorDeca) == 0xCA8);
#endif

// XObjActor's own virtuals, the ones the subclasses call through to
extern "C" {
void fn_80106BE0(XObjActor* object, f32 frame);             // vf164
Vec fn_80107084(XObjActor* object);                         // GetCameraTarget
void fn_80107600(XObjActor* object);                        // vfC8
void fn_80107750(XObjActor* object, u32 frames);            // vf148
BOOL fn_80107F18(XObjActor* object, const CoordInfo& info); // vf9C
void fn_80108014(XObjActor* object);                        // vf94
void fn_801081F0(XObjActor* object);                        // vf98
void fn_801083F0(XObjActor* object, u32 id);                // vf90
}

struct XObjActor3Part {
	u32 _00;            // _00
	u32 _04;            // _04
	u8 _08[0x68 - 0x8]; // _08
};

// pool 5's objects, 48 of them
class XObjActor3 : public XObj {
public:
	// _00-_980 = XObj
	CTpl _980;    // _980
	f32 _B70;     // _B70
	f32 _B74;     // _B74
	f32 _B78;     // _B78, timer
	f32 _B7C;     // _B7C, heading
	s32 _B80;     // _B80, the kind's step
	u32 _B84;     // _B84
	s32 _B88;     // _B88, frame count
	CStatus _B8C; // _B8C
	s32 _B94;     // _B94, command from the event
	s32 _B98;     // _B98, its arg
	s32 _B9C;     // _B9C, its second arg
	s32 _BA0;     // _BA0, its third arg
	u32 _BA4[7];  // _BA4
	f32 _BC0;     // _BC0, window left
	f32 _BC4;     // _BC4, window right
	f32 _BC8;     // _BC8, angle around the owner
	f32 _BCC;     // _BCC
	f32 _BD0;     // _BD0, height above the owner
	f32 _BD4;     // _BD4
	f32 _BD8;     // _BD8, distance from the owner
	CVec _BDC;    // _BDC
	s32 _BEC;     // _BEC
	f32 _BF0;     // _BF0
	f32 _BF4;     // _BF4
	u8 _BF8;      // _BF8
	u8 _BF9[0xBFC - 0xBF9];
	s32 _BFC; // _BFC
	f32 _C00; // _C00
	s32 _C04; // _C04, count of shown pool 17 objects
	u8 _C08;  // _C08
	u8 _C09[0xC0C - 0xC09];
	CPtcl mPtcl;              // _C0C, haisui_nezumi's dust
	CPtclDust3 mPtcls[50];    // _C20
	CTexObj mTex[2];          // _1BC0
	u32 _1DC0[10];            // _1DC0
	s32 mSe;                  // _1DE8, the looping sfx haisui_nezumi plays
	AnimSfx mAnimSfx;         // _1DEC
	XObjActor3Part mParts[8]; // _1DFC
	u8 _213C[0x2140 - 0x213C];

	XObjActor3();

	virtual ~XObjActor3(); // _08

	// the kind handlers in lbl_8022C330, the units still define them as extern "C" fn_s
	void fn_8010BC08(u32 frames);
	void fn_8010BC0C(u32 frames);
	void fn_8010BF7C(u32 frames);
	void fn_8010C304(u32 frames);
	void fn_8010CE80(u32 frames);
	void fn_8010D114(u32 frames);
	void fn_8010D65C(u32 frames);
	void fn_8010F318(u32 frames);
	void fn_8010FF94(u32 frames);
	void fn_801115FC(u32 frames);
	void fn_80111A78(u32 frames);
	void fn_80114060(u32 frames);
	void fn_80114540(u32 frames);
	void fn_8011463C(u32 frames);
	void fn_801146F4(u32 frames);
	void fn_801146F8(u32 frames);
	void fn_801147C0(u32 frames);
	void fn_80114CC8(u32 frames);
	void fn_801151E0(u32 frames);
	void fn_8011529C(u32 frames);
};

typedef void (XObjActor3::*XObjActor3Func)(u32 frames);

extern "C" XObjActor3Func lbl_8022C330[20];
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjActor3) == 0x2130);
#else
STATIC_ASSERT(sizeof(XObjActor3) == 0x2140);
#endif

#endif
