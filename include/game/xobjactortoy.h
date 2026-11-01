#ifndef _GAME_XOBJACTORTOY_H
#define _GAME_XOBJACTORTOY_H

#include "game/xobjactorbase.h"

// XObjActorToy, the actor the toys are, game/xobjactortoy.cpp. the class name is from its
// vtable, the rest has no name in the dol. which toy it is comes from GetDescKind

#include "game/hsd_dobj.h"

extern "C" {
void* GetSpawnData(s32 index);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
void fn_8001E468(CSound* sound, s32 sfx, s32 vol, s32 arg); // a playing sound's volume
void fn_8001E660(CSound* sound, s32 sfx);                   // stops a sound
// shakes the screen, the CScrnQuake in the game object
void fn_800242A8(void* quake, f32 x, f32 y, f32 z, f32 a, f32 b, f32 c, f32 seconds);
}
#define GAME_QUAKE (lbl_8025C400 + 0x965C)

// more of the game object's object pools, see game/scr.h
#define GAME_POOL_9734 ((UnkObjPool*)(lbl_8025C400 + 0x9734)) // 64 of them
#define GAME_POOL_975C ((UnkObjPool*)(lbl_8025C400 + 0x975C)) // 15 of them
#define GAME_POOL_984C ((UnkObjPool*)(lbl_8025C400 + 0x984C)) // 288 of them

// one slot of the robot's item list
struct UnkPlayerItem {
	s16 mId;  // _00
	s16 mNum; // _02
};

// chibi robo, only the item list
class UnkPlayer : public XObj {
public:
	u8 _0980[0x9688 - 0x980];  // _0980
	UnkPlayerItem mItems[256]; // _9688
};
#define GAME_PLAYER    (*(UnkPlayer**)(lbl_8025C400 + 0x96BC))
#define GAME_ITEMCOUNT (*(s32*)(lbl_8025C400 + 0x5D08C))

extern "C" {
u16 fn_8006E44C(s16 item, s32 kind);
s32 fn_80106AC0(void);
s32 fn_80106B50(void);
}

// the script's variables
extern "C" s32 lbl_80367A2C[];

// name from __vt__9CoordInfo, see game/xobjs.h
class CoordInfo {
public:
	void* _00; // _00
	// _04 vtable

	CoordInfo(void* value)
	    : _00(value)
	{
	}
	virtual ~CoordInfo() { } // _08
};

// one of the toy's 4 parts, 0x3C bytes
struct UnkToyPart {
	s16 mId;      // _00
	s32 mState;   // _04
	XObj* mObj;   // _08
	u8 _0C[0x30]; // _0C
};

class XObjActorToy : public XObjActor {
public:
	typedef void (XObjActorToy::*StateFunc)(u32 frames);

	StateFunc mState;     // _C78
	CStatus _C84;         // _C84
	s32 mLoadStep;        // _C8C, which of the two parts vf9C loads next
	XObj mPart0;          // _C90
	XObj mPart1;          // _1610
	s32 _1F90;            // _1F90
	CVec _1F94;           // _1F94
	s32 _1FA4;            // _1FA4
	UnkToyPart mParts[4]; // _1FA8
	CStatus mToyStatus;   // _2098

	virtual ~XObjActorToy();        // _08
	virtual void Release();         // _54
	virtual void Init(u32 id);      // _90
	virtual void vf94();            // _94
	virtual void Reset();           // _98
	virtual BOOL vf9C(void* info);  // _9C
	virtual void vfAC();            // _AC
	virtual void vfC8();            // _C8
	virtual void vf130();           // _130
	virtual void vf148(u32 frames); // _148
	virtual void vf14C(u32 frames); // _14C
	virtual void vf150();           // _150
	virtual void vf158(u32 frames); // _158
	virtual void vf15C();           // _15C
	virtual void vf164(f32 frame);  // _164

	void SetState(StateFunc state)
	{
		mState = state;
		fn_8010706C(this);
	}

	bool fn_8011566C() const;
	void StateA(u32 frames); // 0x80115FD4, the state the script commands start
};
typedef char XObjActorToy_size_check[sizeof(XObjActorToy) == 0x20A0 ? 1 : -1];

#endif
