#ifndef _GAME_XOBJACTORJENNY_H
#define _GAME_XOBJACTORJENNY_H

#include "game/xobjactorbase.h"

// XObjActorJenny, game/xobjactorjenny.cpp. the class name is from its vtable, the rest has
// no name in the dol

class XObjActorJenny : public XObjActor {
public:
	typedef void (XObjActorJenny::*StateFunc)(u32 frames);

	StateFunc mState; // _C78
	f32 _C84;         // _C84
	f32 _C88;         // _C88
	s32 _C8C;         // _C8C
	CStatus _C90;     // _C90
	CStatus _C98;     // _C98
	u8 _CA0;          // _CA0

	virtual ~XObjActorJenny();          // _08
	virtual void UpdateAnim(u32 speed); // _88
	virtual void vf15C();               // _15C
	virtual void vf148(u32 frames);     // _148
	virtual void vf14C(u32 frames);     // _14C
	virtual void vf150();               // _150
	virtual HSD_JObj* vf168();          // _168
	virtual CVec vf16C();               // _16C
	virtual f32 vf178();                // _178
	virtual f32 vf17C();                // _17C

	void StateIdle(u32 frames); // 0x801232A8
	void Drop();
};
typedef char XObjActorJenny_size_check[sizeof(XObjActorJenny) == 0xCA8 ? 1 : -1];

#endif
