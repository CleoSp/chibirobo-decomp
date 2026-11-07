#ifndef _GAME_XOBJ_H
#define _GAME_XOBJ_H

#include "game/mapfloorhit.h"
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

	virtual ~CAnmQueue() { } // _08

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
	Vec mContactOffset; // _00
	Vec mFrom;          // _0C
	Vec mTo;            // _18
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
	f32 mFloorHeights[32];       // _6FC, a height per floor
	f32 _77C;                    // _77C
	f32 mSelectedFloorHeight;    // _780, highest active floor, NaN when none is active
	s32 mSelectedFloorIndex;     // _784
	XObjMapHit mFloorHit;        // _788
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

	virtual ~XObj() { }     // _08
	virtual void Release(); // _54
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate);                   // _80
	virtual void ClearAnim();                              // _84
	virtual void vf90(u32 id);                             // _90, init
	virtual void vf94();                                   // _94
	virtual void ResetObjectState();                       // _98, resets the move state
	virtual BOOL LoadFromPlacement(const CoordInfo& info); // _9C, called until it returns true
	virtual BOOL vfA0(const char* path);                   // _A0, reads the second archive
	virtual void UpdateObject(u32 frames);                 // _A4, update
	virtual void vfA8(u32 frames);                         // _A8, steps mColorStep
	virtual void vfAC();                                   // _AC
	virtual void vfB0(Mtx view);                           // _B0, draw
	virtual void vfB4(Mtx view);                           // _B4
	virtual void vfB8();                                   // _B8
	virtual void vfBC(const Vec* origin, f32 opacity);     // _BC
	virtual void vfC0();                                   // _C0
	virtual void vfC4();                                   // _C4
	virtual void vfC8();                                   // _C8
	virtual void vfCC();                                   // _CC
	virtual void vfD0(CShadow* shadow);                    // _D0
	virtual void vfD4(CShadow* shadow);                    // _D4
	virtual void vfD8(u32* flags);                         // _D8
	virtual void vfDC(BOOL enabled, u8 value);             // _DC
	virtual void vfE0(f32 factor);                         // _E0
	virtual void vfE4(f32 alpha);                          // _E4
	virtual void vfE8();                                   // _E8
	virtual Vec GetCameraTarget();                         // _EC, where a camera looks at this object
	virtual HSD_AnimJoint* vfF0(s32 id);                   // _F0, joint anim
	virtual HSD_MatAnimJoint* vfF4(s32 id);                // _F4, material anim
	virtual HSD_ShapeAnimJoint* vfF8(s32 id);              // _F8, shape anim
	virtual u32 vfFC(const Vec* from, const Vec* to, f32 radius, f32 height, Vec* push, Vec* contact, s32 arg6,
	                 s32 arg7);                                                                 // _FC, collision test
	virtual s32 vf100(s32 unused, s32 value);                                                   // _100
	virtual BOOL TestFloorCrossing(const Vec& start, const Vec& end);                           // _104
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
	virtual BOOL vf134(f32 amount);                                                             // _134
	virtual void vf138(const Vec& position, f32 strength);                                      // _138, hit from a position
	virtual void vf13C(const Vec* contact, s32 value, s32 tint);                                // _13C
	virtual BOOL vf140(const Vec* position, s32 value);                                         // _140
	virtual void vf144(s32 unused, s32 volume);                                                 // _144, the mic volume

	// todo: cleanup
	s32 GetKind() const { return mAttr._04 != NULL ? mAttr._04->_00 : -1; }

	// which anim archive an anim id is in, 0 for the model's own
	s32 GetAnimSet(s32 id) const
	{
		bool ret = false;
		if (id >= 1000 && mAnim.mArchive != NULL) {
			ret = true;
		}
		return ret ? id / 1000 : 0;
	}

	BOOL IsFloorOn(int i) const { return _250[0] & (1 << i); }
	void ClearFloor(int i) { _250[0] &= ~(1 << i); }

	BOOL HasQueuedAnim() const
	{
		for (int i = 0; i < 16; i++) {
			if (mAnmQueue.mEntry[(mAnmQueue.mNext + i) % 16].mId != -1) {
				return TRUE;
			}
		}
		return FALSE;
	}

	// takes the next anim off the queue
	void PopAnim(s32& id, u16& frame, s16& rate)
	{
		for (int i = 0; i < 16; i++) {
			s32 j = (mAnmQueue.mNext + i) % 16;
			if (mAnmQueue.mEntry[j].mId != -1) {
				id                      = mAnmQueue.mEntry[j].mId;
				frame                   = mAnmQueue.mEntry[j].mFrame;
				rate                    = mAnmQueue.mEntry[j].mRate;
				mAnmQueue.mEntry[j].mId = -1;
				mAnmQueue.mNext         = (j + 1) % 16;
				return;
			}
		}
	}

	s32 GetPool() const { return mAttr._00 != NULL ? GetObjDataPool(mAttr._00) : -1; }

	// the kind record's category and item, s16 like the record's fields
	static s16 GetDataCategory(const UnkObjData* data)
	{
		if (data != NULL) {
			return data->_14;
		}
		return 0;
	}
	static s16 GetDataItem(const UnkObjData* data)
	{
		if (data != NULL) {
			return data->_16;
		}
		return 0;
	}
	s16 GetCategory() const
	{
		if (mAttr._00 != NULL) {
			return GetDataCategory(mAttr._00);
		}
		return -1;
	}
	s16 GetItem() const
	{
		if (mAttr._00 != NULL) {
			return GetDataItem(mAttr._00);
		}
		return -1;
	}

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
XObj* __dt__4XObjFv(XObj* object, s32 flags);
BOOL LoadJointAnimationAsync(CJAnim* anim, const char* path);
// the default vf138
void vf138__4XObjFRC3Vecf(XObj* object, const Vec* position, f32 strength);
CVec GetFloorContactPosition(XObj* object, s32 floor, const Vec* offset);
// the default vf12C
void vf12C__4XObjFlUcUcUcf(XObj* object, s32 blend, u8 red, u8 green, u8 blue, f32 time);
f32 GetFloorFacingAngle(XObj* object, const Vec* pos, s32 index, s32 requireSegment, Vec* contact, Vec* normalOut);
void UpdateObjectFloorHeights(XObj* object);
void fn_8008D220(XObj* object);
void SetAnim__4XObjFliPvPvPvffff(XObj* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                                 f32 blend, f32 blendRate);
void vfC4__4XObjFv(XObj* object);
void vfC8__4XObjFv(XObj* object);
void vfBC__4XObjFPC3Vecf(XObj* object, const Vec* origin, f32 opacity);
void vfB8__4XObjFv(XObj* object);
void vfB0__4XObjFPA4_f(XObj* object, f32 (*view)[4]);
void UpdateObject__4XObjFUl(XObj* object, u32 frames);
void Release__4XObjFv(XObj* object);
void vfDC__4XObjFiUc(CXObj* object, BOOL enabled, u8 mode);
// walk callback, sets up each material's pe and toon flags
void fn_80091888(HSD_JObj* joint, f32** arguments, s32 type);
// the default LoadFromPlacement, reads the object's coord info
BOOL LoadFromPlacement__4XObjFRC9CoordInfo(XObj* object, const CoordInfo* info);
void ResetObjectState__4XObjFv(XObj* object);
void vf90__4XObjFUl(XObj* object, u32 id);

// the object kinds from 0x800D4000 to 0x80108000
void SyncMoveFromJoint(CXObj* object);
void UpdateSpiderCoinBounces(void* context, u32 frames);
void ResetSpiderCoinBounces(void* context);
void ResetSpiderBossState(XObj* object);
void fn_8010706C(void* object);
s32 vf124__9XObjActorFlPl(CXObj* object, s32 index, s32* out);
void SetActorLookAtPosition(CXObj* object, const Vec* target, f32 rate);
void SetActorLookAngle(CXObj* object, f32 value, f32 rate);
void SetActorLookTarget(CXObj* object, const Vec* target, f32 rate);
void ResetActorLook(CXObj* object, f32 rate);
void UpdateObject__9XObjActorFUl(XObj* object, u32 frames);

// pools and counters the object kinds share
extern XObj* gSpiderCarryModels[16];
extern s32 gSpiderCarrySlotOwners[16];
extern XObj* gSmallSpiderEffectObjects[12];
extern XObj* lbl_8065B2A0;
extern XObj* lbl_8065B2AC;
extern CStatus lbl_8065B2B0;
extern u8 lbl_8065B2D4;
extern s32 lbl_8065B2D8;
extern XObj* lbl_8065B2DC;
extern u8 lbl_8065B2E0;
extern u32 lbl_8065B2E8;
extern s32 lbl_8065B2F0;
extern s32 lbl_8065B2F4;
}

extern "C" void HSD_MtxGetTranslate(Mtx mat, Vec* vec);

// the jobj's place in the world
inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// what an object kind's objTable entry starts with, the archive its model is read from
struct XObjSource {
	const char* mName;     // _00
	CObjArchive* mArchive; // _04
	u32 _08;
	struct Cache {
		u32 address;
		u32 size;
	} mCache;
};

// todo: cleanup, coordinfo.h has UnkObjData's first word as padding
inline XObjSource*& GetObjDataSource(UnkObjData* data)
{
	return *(XObjSource**)data->_00;
}

#endif
