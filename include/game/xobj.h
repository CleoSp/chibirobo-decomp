#ifndef _GAME_XOBJ_H
#define _GAME_XOBJ_H

#include "game/cobj.h"
#include "game/ccamera.h"
#include "game/coordinfo.h"
#include "game/evt.h"

// class names from RTTI and the vtable at 0x80225984, the virtuals past CXObjIntp's are unnamed.
// offsets are usa, japan's CJArchive is 8 bytes shorter so everything here sits 8 lower there
// and anything after mAnim, another CJArchive, 0x10 lower

class CShadow;

struct CAnmQueueEntry {
	s32 mId;    // _00, -1 when free
	u16 mFrame; // _04
	s16 mRate;  // _06
};

// anims waiting to play after the current one, name from __vt__9CAnmQueue
class CAnmQueue {
public:
	s32 mNext;                 // _00
	CAnmQueueEntry mEntry[16]; // _04
	// _84 vtable

	virtual ~CAnmQueue(); // _08

	// todo: cleanup
	void Clear()
	{
		mNext = 0;
		for (u32 i = 0; i < 16; ++i) {
			mEntry[i].mId = -1;
		}
	}
};
typedef char CAnmQueue_size_check[sizeof(CAnmQueue) == 0x88 ? 1 : -1];

// one of an object's floors, a list of them ends at an id of -1
struct XObjFloor {
	s16 mId;    // _00
	s16 mType;  // _02, ground type, 13 and up count as none
	s32 _04[3]; // _04, in hundredths
	s32 _10;    // _10
	s32 _14[3]; // _14, in hundredths
};
typedef char XObjFloor_size_check[sizeof(XObjFloor) == 0x20 ? 1 : -1];

// a floor's edge, offsets from the object's position
struct XObjEdge {
	Vec _00;   // _00
	Vec mFrom; // _0C
	Vec mTo;   // _18
};
typedef char XObjEdge_size_check[sizeof(XObjEdge) == 0x24 ? 1 : -1];

// a material color stepping towards a target, the two of them end up in CXObj's mColors
struct XObjColorStep {
	Vec mValue;  // _00
	Vec mTarget; // _0C
	Vec mSpeed;  // _18
};
typedef char XObjColorStep_size_check[sizeof(XObjColorStep) == 0x24 ? 1 : -1];

// the game's object, everything in a stage that has a model
class XObj : public CXObjIntp {
public:
	// _00-_228 = CXObjIntp
	CStatus64 mStatus;           // _228, 1 in use, 0x10000 drawn, 0x0040000000000000 while a read is pending
	CoordInfo mCoord;            // _238
	AttrInfo mAttr;              // _240
	XObjFloor* mFloors;          // _24C
	u32 _250[2];                 // _250, a bit per floor, one mask per group
	s32 _258;                    // _258, index in its pool
	s32 _25C;                    // _25C
	s32 _260;                    // _260
	CVec _264;                   // _264, POS copies the position here
	s32 _274;                    // _274
	XObjEdge mEdges[32];         // _278, one per floor
	f32 _6F8;                    // _6F8
	f32 _6FC[32];                // _6FC, a height per floor
	f32 _77C;                    // _77C
	f32 _780;                    // _780, highest _6FC under the _250[0] mask
	s32 _784;                    // _784, the floor that is
	s32 _788;                    // _788
	u32 _78C;                    // _78C
	f32 _790;                    // _790
	CVec _794;                   // _794
	CEvt mEvt;                   // _7A4, the object's own script
	u32 _80C;                    // _80C
	CAnmQueue mAnmQueue;         // _810
	f32 mAnchorX;                // _898, NAN while there's no anchor to clamp around
	f32 mAnchorZ;                // _89C
	f32 mLimitPosX;              // _8A0
	f32 mLimitNegX;              // _8A4
	f32 mLimitPosZ;              // _8A8
	f32 mLimitNegZ;              // _8AC
	u32 _8B0;                    // _8B0, clamp flags from the event call -11, 0x80000000 while it's off
	XObjColorStep mColorStep[2]; // _8B4
	CJAnim mAnim;                // _8FC, anims from the object's second archive, ids from 1000 up

	XObj(); // fn_8001AFAC, a weak copy

	virtual ~XObj();        // _08
	virtual void Release(); // _54
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate);               // _80
	virtual void ClearAnim();                          // _84
	virtual void vf90(u32 id);                         // _90, init
	virtual void vf94();                               // _94
	virtual void vf98();                               // _98, resets the move state
	virtual BOOL vf9C(const CoordInfo& info);          // _9C, called until it returns true
	virtual BOOL vfA0(const char* path);               // _A0, reads the second archive
	virtual void vfA4(u32 frames);                     // _A4, update
	virtual void vfA8(u32 frames);                     // _A8, steps mColorStep
	virtual void vfAC();                               // _AC
	virtual void vfB0(Mtx view);                       // _B0, draw
	virtual void vfB4(Mtx view);                       // _B4
	virtual void vfB8();                               // _B8
	virtual void vfBC(const Vec* origin, f32 opacity); // _BC
	virtual void vfC0();                               // _C0
	virtual void vfC4();                               // _C4
	virtual void vfC8();                               // _C8
	virtual void vfCC();                               // _CC
	virtual void vfD0(CShadow* shadow);                // _D0
	virtual void vfD4(CShadow* shadow);                // _D4
	virtual void vfD8(u32* flags);                     // _D8
	virtual void vfDC(BOOL enabled, u32 value);        // _DC
	virtual void vfE0(f32 factor);                     // _E0
	virtual void vfE4(f32 alpha);                      // _E4
	virtual void vfE8();                               // _E8
	virtual Vec GetCameraTarget();                     // _EC, where a camera looks at this object
	virtual HSD_AnimJoint* vfF0(s32 id);               // _F0, joint anim
	virtual HSD_MatAnimJoint* vfF4(s32 id);            // _F4, material anim
	virtual HSD_ShapeAnimJoint* vfF8(s32 id);          // _F8, shape anim
	virtual u32 vfFC(const Vec* from, const Vec* to, f32 radius, f32 height, Vec* push, Vec* contact, s32 arg6,
	                 s32 arg7);                                                                 // _FC, collision test
	virtual s32 vf100(s32 unused, s32 value);                                                   // _100
	virtual void vf104();                                                                       // _104
	virtual f32 vf108();                                                                        // _108, shadow radius
	virtual void vf10C(const Vec* offset, f32* x, f32* y, f32* z, CCamera* camera);             // _10C, a point near the object on screen
	virtual f32 vf110();                                                                        // _110
	virtual f32 vf114();                                                                        // _114
	virtual BOOL vf118(f32* width, f32* height, CCamera* camera, f32 radius, f32 objectHeight); // _118
	virtual BOOL vf11C(CCamera* camera);                                                        // _11C
	virtual void vf120(s32 mode);                                                               // _120, polygon rendering mode
	virtual s32 vf124(s32 count, s32* args);                                                    // _124
	virtual const void* vf128();                                                                // _128, script passed to an object event
	virtual void vf12C(s32 blend, u8 r, u8 g, u8 b, f32 time);                                  // _12C, interpolates a material color
	virtual void vf130();                                                                       // _130
	virtual BOOL vf134();                                                                       // _134
	virtual void vf138(const Vec& position, f32 strength);                                      // _138, hit from a position
	virtual void vf13C(s32 unused, s32 value);                                                  // _13C
	virtual BOOL vf140(s32 unused, s32 value);                                                  // _140
	virtual void vf144();                                                                       // _144

	// todo: cleanup
	s32 GetKind() const { return mAttr._04 != NULL ? mAttr._04->_00 : -1; }
	s32 GetPool() const { return mAttr._00 != NULL ? GetObjDataPool(mAttr._00) : -1; }
	s32 GetObjFlags() const { return mAttr._00 != NULL ? GetObjDataFlags(mAttr._00) : -1; }
};
#if defined(VERSION_JP)
typedef char XObj_size_check[sizeof(XObj) == 0x970 ? 1 : -1];
#else
typedef char XObj_size_check[sizeof(XObj) == 0x980 ? 1 : -1];
#endif

// the object that plays camera animations
class XObjCamera : public XObj {
public:
	CCamera mCamera; // _980
	s32 mCameraAnim; // _B14, -1 when no anim is playing

	XObjCamera();
};

class CJArchive;
class CXObj;
class CStatus;

// the object base functions, the units from 0x8008A000 to 0x80093000
extern "C" {
// ~XObj, the subclass dtors call it with 0
XObj* fn_8000877C(XObj* object, s32 flags);
BOOL fn_8008A38C(CJArchive* archive, const char* path);
void fn_8008A6C0(XObj* object, f32 value);
CVec fn_8008C274(CXObj* object, s32 floor, const Vec* offset);
// the default vf12C
void fn_8008A820(XObj* object, s32 blend, u8 red, u8 green, u8 blue, f32 time);
void fn_8008C8A4(XObj* object);
void fn_8008D220(XObj* object);
void fn_800902A4(XObj* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate);
void fn_8009047C(XObj* object);
void fn_8009056C(XObj* object, const Vec* origin, f32 opacity);
void fn_80090864(XObj* object);
void fn_800908C0(XObj* object, f32 (*view)[4]);
void fn_80090ED4(XObj* object, u32 frames);
void fn_800911C0(XObj* object);
void fn_80091834(CXObj* object, BOOL enabled, u32 value);
// walk callback, sets up each material's pe and toon flags
void fn_80091888(HSD_JObj* joint, f32** arguments, s32 type);
// the default vf9C, reads the object's coord info
BOOL fn_80091C80(XObj* object, const CoordInfo* info);
void fn_800928E8(XObj* object);
void fn_800929C4(XObj* object, u32 id);

// the object kinds from 0x800D4000 to 0x80108000
void fn_800D40EC(CXObj* object);
void fn_800EA780(void* context, u32 frames);
void fn_800EA99C(void* context);
void fn_800F38F4(XObj* object);
void fn_800F4350(XObj* object);
void fn_8010660C(XObj* object);
void fn_8010706C(void* object);
s32 fn_801070A4(CXObj* object, s32 index, s32* out);
void fn_801076B0(CXObj* object, s32 value, f32 rate);
void fn_801076DC(CXObj* object, f32 value, f32 rate);
void fn_801076FC(CXObj* object, s32 value, f32 rate);
void fn_8010771C(CXObj* object, f32 rate);
void fn_80107DE0(XObj* object, u32 frames);

// pools and counters the object kinds share
extern XObj* lbl_8037E5C0[16];
extern s32 lbl_8037E600[16];
extern XObj* lbl_8037E910[12];
extern XObj* lbl_8065B2A0;
extern XObj* lbl_8065B2AC;
extern CStatus lbl_8065B2B0;
extern u8 lbl_8065B2D4;
extern s32 lbl_8065B2D8;
extern u32 lbl_8065B2DC;
extern u8 lbl_8065B2E0;
extern u32 lbl_8065B2E8;
extern s32 lbl_8065B2F0;
extern s32 lbl_8065B2F4;
}

#endif
