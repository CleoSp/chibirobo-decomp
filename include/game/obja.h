#ifndef _GAME_OBJA_H
#define _GAME_OBJA_H

#include "game/cobj.h"
#include "game/cjobjset.h"

// XObj and the classes on it, game/obja*.cpp. class names from RTTI and vtable symbols,
// nothing else has a name. offsets are usa

extern "C" {
void HSD_JObjModifyFlags(HSD_JObj* jobj, u32 mask, u32 flags);
void HSD_DObjModifyFlags(HSD_DObj* dobj, u32 mask, u32 flags);
void HSD_MObjSetAlpha(HSD_MObj* mobj, f32 alpha);
void JObjSetPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
void JObjClearPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
}
extern "C" void HSD_MtxGetTranslate(Mtx mat, Vec* vec);

// the jobj's place in the world
inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// one 0x20 byte entry of an object's point table, ends with a kind of -1
struct XObjPoint {
	s16 mKind;    // _00
	s16 _02;      // _02
	s32 mPos[3];  // _04, hundredths
	s32 mRotY;    // _10, degrees
	s32 mPos2[3]; // _14, hundredths
};

// what the object was placed from
struct XObjSource {
	u8 _00[0x4];           // _00
	CObjArchive* mArchive; // _04
};

struct XObjInfo {
	XObjSource* mSource; // _00
	u8 _04[0x10];        // _04
	s16 mId;             // _14
};

inline s16 GetInfoId(XObjInfo* info)
{
	if (info != NULL) {
		return info->mId;
	}
	return 0;
}

// a color stepping to a target, 0 to 255 per channel
struct XObjColor {
	Vec mColor;  // _00
	Vec mTarget; // _0C
	Vec mSpeed;  // _18
};

// one slot of the object's message table
struct XObjMsg {
	s32 mId;    // _00, an anim id, or two in the halves
	u16 mFrame; // _04
	s16 mRate;  // _06, hundredths
};

struct XObjEvt {
	u8 _00[0x4];  // _00
	void* mEvt;   // _04
	u8 _08[0x60]; // _08
};

// __vt__4XObj, the object every placed thing in the house is
class XObj : public CXObjIntp {
public:
	u64 mFlags;             // _228
	u8 _230[0x8];           // _230
	s32 _238;               // _238
	u8 _23C[0x4];           // _23C
	XObjInfo* mInfo;        // _240
	s32 _244;               // _244
	u8 _248[0x4];           // _248
	XObjPoint* mPoints;     // _24C
	u32 mPointMasks[2];     // _250, the points on now and before
	s32 _258;               // _258
	u8 _25C[0xC];           // _25C
	f32 _268;               // _268, the y the floor test last ran at
	u8 _26C[0x6FC - 0x26C]; // _26C
	f32 mPointDist[32];     // _6FC
	f32 _77C;               // _77C
	f32 mNearDist;          // _780
	s32 mNearPoint;         // _784
	s32 _788;               // _788
	s32 _78C;               // _78C
	f32 _790;               // _790
	f32 _794;               // _794
	f32 _798;               // _798
	f32 _79C;               // _79C
	u8 _7A0[0x4];           // _7A0
	XObjEvt mEvt;           // _7A4
	s32 _80C;               // _80C
	s32 mMsgCount;          // _810, where the ring starts
	XObjMsg mMsgs[16];      // _814
	f32 _894;               // _894
	f32 _898;               // _898, the anchor it is held around, nan when free
	f32 _89C;               // _89C
	f32 mMinX;              // _8A0, the box around the anchor, in its own turn
	f32 mMaxX;              // _8A4
	f32 mMinZ;              // _8A8
	f32 mMaxZ;              // _8AC
	u32 _8B0;               // _8B0
	XObjColor mColors[2];   // _8B4
	CJAnim mAnim;           // _8FC

	virtual void Init(u32 id);                                     // _90
	virtual void vf94();                                           // _94
	virtual void Reset();                                          // _98
	virtual BOOL vf9C(void* info);                                 // _9C
	virtual void vfA0();                                           // _A0
	virtual void Update(u32 frames);                               // _A4
	virtual void vfA8();                                           // _A8
	virtual void vfAC();                                           // _AC
	virtual void Draw(MtxPtr vmtx);                                // _B0
	virtual void DrawSub(void* args);                              // _B4
	virtual void vfB8();                                           // _B8
	virtual void vfBC(Vec* pos, f32 scale);                        // _BC
	virtual void vfC0();                                           // _C0
	virtual void vfC4();                                           // _C4
	virtual void vfC8();                                           // _C8
	virtual void vfCC();                                           // _CC
	virtual void vfD0();                                           // _D0
	virtual void vfD4();                                           // _D4
	virtual void vfD8(void* args);                                 // _D8
	virtual void vfDC(s32 a, u8 b);                                // _DC
	virtual void vfE0(f32 alpha);                                  // _E0
	virtual void vfE4(f32 alpha);                                  // _E4
	virtual void vfE8();                                           // _E8
	virtual void vfEC();                                           // _EC
	virtual void* GetAnimJoint(s32 id);                            // _F0
	virtual void* GetMatAnimJoint(s32 id);                         // _F4
	virtual void* GetShapeAnimJoint(s32 id);                       // _F8
	virtual void vfFC();                                           // _FC
	virtual void vf100();                                          // _100
	virtual void vf104();                                          // _104
	virtual void vf108();                                          // _108
	virtual void vf10C(Vec* pos, f32* x, f32* y, f32* z, s32 arg); // _10C, a point near the object on screen
	virtual void vf110();                                          // _110
	virtual void vf114();                                          // _114
	virtual void vf118();                                          // _118
	virtual void vf11C();                                          // _11C
	virtual void vf120(s32 mode);                                  // _120
	virtual void vf124();                                          // _124
	virtual s32 GetSoundPos();                                     // _128
	virtual void SetColor(s32 i, u8 r, u8 g, u8 b, f32 seconds);   // _12C
	virtual void vf130();                                          // _130
	virtual BOOL vf134();                                          // _134
	virtual void vf138(f32 dist);                                  // _138
	virtual BOOL vf13C(s32 a, s32 b);                              // _13C
	virtual BOOL vf140(s32 a, s32 b);                              // _140
	virtual void vf144();                                          // _144

	// which anim archive an anim id is in, 0 for the model's own
	s32 GetAnimSet(s32 id) const
	{
		bool ret = false;
		if (id >= 1000 && mAnim.mArchive != NULL) {
			ret = true;
		}
		return ret ? id / 1000 : 0;
	}

	// the id of what it was placed from, -1 when it wasn't
	s16 GetId() const
	{
		if (mInfo != NULL) {
			return GetInfoId(mInfo);
		}
		return -1;
	}

	BOOL IsPointOn(int i) const { return mPointMasks[0] & (1 << i); }
	void PointOff(int i) { mPointMasks[0] &= ~(1 << i); }

	BOOL HasMsg() const
	{
		for (int i = 0; i < 16; i++) {
			if (mMsgs[(mMsgCount + i) % 16].mId != -1) {
				return TRUE;
			}
		}
		return FALSE;
	}

	void PopMsg(s32& id, u16& frame, s16& rate)
	{
		for (int i = 0; i < 16; i++) {
			s32 j = (mMsgCount + i) % 16;
			if (mMsgs[j].mId != -1) {
				id           = mMsgs[j].mId;
				frame        = mMsgs[j].mFrame;
				rate         = mMsgs[j].mRate;
				mMsgs[j].mId = -1;
				mMsgCount    = (j + 1) % 16;
				return;
			}
		}
	}

	// clears the message table
	void ClearMsgs()
	{
		mMsgCount = 0;
		for (int i = 0; i < 16; i++) {
			mMsgs[i].mId = -1;
		}
	}
};
typedef char XObj_size_check[sizeof(XObj) == 0x980 ? 1 : -1];

// the XObj methods the classes on it call directly
extern "C" {
void fn_800911C0(XObj* obj);             // Release
BOOL fn_80091C80(XObj* obj, void* info); // vf9C, loads the model
void fn_800929C4(XObj* obj, u32 id);     // Init
}

// what the stage table says about the current stage
struct UnkStageInfo {
	u8 _00[0x4];    // _00
	u8 mLightMode;  // _04, 2 when the stage lights come from the objects
	u8 mShadowMask; // _05, bit 7 - i casts shadows for light i
};

struct UnkStageRef {
	UnkStageInfo* mInfo; // _00
};

extern "C" UnkStageRef* fn_8006F4B8(void);

#endif
