#ifndef _GAME_XOBJSPIDERSMALL_H
#define _GAME_XOBJSPIDERSMALL_H

#include "game/xobj.h"
#include "game/csound.h"

// the small spiders, npc_spider_2, pool 15. class name from RTTI and the vtable at 0x8022B308,
// the units from 0x800F49F0 to 0x80100264. offsets are usa, everything past XObj sits 0x10 lower
// in japan

// the fx_spider_01 the spider carries around, drawn from lbl_8037E910
struct XObjSpiderSmallFx {
	CVec mPos;       // _00
	CVec _10;        // _10
	f32 mRotY;       // _20
	f32 _24;         // _24
	s32 mState;      // _28
	u32 _2C;         // _2C
	CStatus mStatus; // _30, 1 shown
};
typedef char XObjSpiderSmallFx_size_check[sizeof(XObjSpiderSmallFx) == 0x38 ? 1 : -1];

class XObjSpiderSmall : public XObj {
public:
	// _00-_980 = XObj
	f32 _980;               // _980
	f32 _984;               // _984
	f32 _988;               // _988
	f32 _98C;               // _98C
	u8 _990[0x994 - 0x990]; // _990
	f32 _994;               // _994
	f32 mRadius;            // _998
	f32 _99C;               // _99C
	f32 _9A0;               // _9A0
	f32 _9A4;               // _9A4
	s32 mState;             // _9A8
	s32 mStep;              // _9AC
	s32 mTimer;             // _9B0
	CStatus mSpiderStatus;  // _9B4, 1 hit, 2 knocked over, 0x40 shadow, 0x800000 grabbed
	s32 mPrevState;         // _9BC
	u32 _9C0;               // _9C0
	f32 _9C4;               // _9C4
	f32 _9C8;               // _9C8
	Vec mHitPos;            // _9CC
	Vec _9D8;               // _9D8
	f32 mHitStrength;       // _9E4
	s32 _9E8;               // _9E8
	s32 _9EC;               // _9EC
	Vec _9F0;               // _9F0
	Vec _9FC;               // _9FC
	Vec mAreaPos;           // _A08, corner of the area it walks in
	f32 mAreaWidth;         // _A14
	f32 mAreaDepth;         // _A18
	Vec mTargetPos;         // _A1C
	f32 mTargetRadius;      // _A28
	s32 mStage;             // _A2C
	s32 mEvtArgs[6];        // _A30, from vf124
	u32 _A48;               // _A48
	XObjSpiderSmallFx mFx;  // _A4C
	u32 _A84;               // _A84
	u8 mFxIndex;            // _A88, into lbl_8037E910, 255 for none
	u8 _A89[0xA8C - 0xA89]; // _A89
	f32 _A8C;               // _A8C
	f32 _A90;               // _A90
	u32 _A94;               // _A94
	f32 _A98;               // _A98
	Vec _A9C;               // _A9C
	u8 _AA8;                // _AA8
	u8 _AA9[0xAAC - 0xAA9]; // _AA9
	s32 _AAC;               // _AAC
	u8 _AB0[0xAB4 - 0xAB0]; // _AB0
	s32 _AB4;               // _AB4, pool index it loaded with
	s32 mTimeMode;          // _AB8
	u32 _ABC;               // _ABC
	u32 _AC0;               // _AC0
	u32 _AC4;               // _AC4
	AnimSfx mAnimSfx;       // _AC8

	XObjSpiderSmall();

	virtual ~XObjSpiderSmall();                            // _08
	virtual void Release();                                // _54
	virtual void vf90(u32 id);                             // _90
	virtual void vf94();                                   // _94
	virtual void vf98();                                   // _98
	virtual BOOL vf9C(const CoordInfo& info);              // _9C
	virtual void vfA4(u32 frames);                         // _A4
	virtual void vfAC();                                   // _AC
	virtual void vfB8();                                   // _B8
	virtual void vfBC(const Vec* origin, f32 opacity);     // _BC
	virtual void vfC0();                                   // _C0
	virtual void vfC8();                                   // _C8
	virtual void vfCC();                                   // _CC
	virtual s32 vf100(s32 unused, s32 value);              // _100
	virtual s32 vf124(s32 count, s32* args);               // _124
	virtual void vf138(const Vec& position, f32 strength); // _138
	virtual void vf148();                                  // _148, anim sounds
};
#if defined(VERSION_JP)
typedef char XObjSpiderSmall_size_check[sizeof(XObjSpiderSmall) == 0xAC8 ? 1 : -1];
#else
typedef char XObjSpiderSmall_size_check[sizeof(XObjSpiderSmall) == 0xAD8 ? 1 : -1];
#endif

// the spider file's bss, one block the code reaches from lbl_8037E7D0
struct XObjSpiderSmallWork {
#if defined(VERSION_JP)
	u8 _000[0x134]; // _000
#else
	u8 _000[0x140]; // _000
#endif
	XObj* mFx[12];           // _140, lbl_8037E910
	s32 _170[12];            // _170, lbl_8037E940
	u8 _1A0[0xC];            // _1A0, lbl_8037E970, mStatus's destructor chain link
	CStatus mStatus[39][16]; // _1AC, lbl_8037E97C, a row per stage, a spider per pool index
};

extern "C" {
XObjSpiderSmall* __dt__15XObjSpiderSmallFv(XObjSpiderSmall* spider, s32 flags);
void fn_800F4DB8(XObjSpiderSmallFx* fx, const Vec* pos);
u8 fn_800F5678(XObjSpiderSmall* spider);
u8 fn_800F5684(XObjSpiderSmall* spider);
void fn_800FF168(XObjSpiderSmall* spider);
void fn_800FFB00(XObjSpiderSmall* spider);

// lbl_8037E97C's construct and destroy helpers
void fn_8010011C();
CStatus* fn_80100154(CStatus* row, s32 flags);
CStatus* fn_801001B4(CStatus* status, s32 flags);
CStatus* fn_80100204(CStatus* row);
CStatus* fn_8010024C(CStatus* status);

extern XObjSpiderSmallWork lbl_8037E7D0;
extern s32 lbl_8037E940[12];
extern u8 lbl_8037E970[12]; // lbl_8037E97C's DestructorChain
extern CStatus lbl_8037E97C[39][16];
extern AnimSfxEvent lbl_8022B170[]; // the anim sound events
}

#endif
