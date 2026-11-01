#ifndef _GAME_XOBJACTORFAMILY_H
#define _GAME_XOBJACTORFAMILY_H

#include "game/xobjactorbase.h"

// the house's people, XObjActorMama, XObjActorPapa, XObjActorTao and XObjActorDeca, one file
// each. class names are from their vtables, the methods are named after the XObj and
// XObjActor slots they fill, the rest after their addresses

// more of XObjActor's methods, game/xobjactorupdate.cpp and around
extern "C" {
void fn_801076FC(XObjActor* self, const Vec* target, f32 rate); // turn to look at target
void fn_801076B0(XObjActor* self, const Vec* target, f32 rate); // the same, and 0x400
void fn_80107DE0(XObjActor* self, u32 frames);                  // Update
Vec fn_80107084(XObjActor* self);                               // GetCameraTarget
}

// shakes the screen, see game/xobjactortoy.h
extern "C" void fn_800242A8(void* quake, f32 x, f32 y, f32 z, f32 a, f32 b, f32 c, f32 seconds);
#define GAME_QUAKE (lbl_8025C400 + 0x965C)

// the event manager's, GAME_EVTMGR
extern "C" void fn_800660E8(void* mgr, s32 on, s32 b);

// where a joint of the object is, found by its path down the tree
extern "C" CVec fn_8007E7C8(XObj* obj, s32 depth, s32* path);

// the joint at the end of the path, one child step then path[i] sibling steps per level
static inline HSD_JObj* FindJoint(HSD_JObj* jobj, s32 depth, s32* path)
{
	s32 i;
	s32 j;

	if (jobj == NULL) {
		return NULL;
	}
	for (i = 0; i < depth; i++, path++) {
		jobj = jobj == NULL ? NULL : jobj->child;
		for (j = 0; j < *path; j++) {
			jobj = jobj == NULL ? NULL : jobj->next;
			if (jobj == NULL) {
				return NULL;
			}
		}
	}
	return jobj;
}

extern "C" void HSD_MtxGetScale(Mtx mat, Vec* vec);

// see game/xobjs.h
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

struct UnkXObjList;
extern "C" void* fn_8006EDB4(s32 id);
extern "C" XObj* fn_80132698(UnkXObjList* list, CoordInfo* info, s32 unused, BOOL async);
#define GAME_OBJLIST ((UnkXObjList*)(lbl_8025C400 + 0x9694))

// starts reading the object id names, see game/objb_act.cpp's fn_800D3D0C
static inline XObj* ReadObj(s16 id)
{
	return fn_80132698(GAME_OBJLIST, &CoordInfo(fn_8006EDB4(id)), 0, TRUE);
}

// the scale of the object as it is drawn
extern "C" CVec fn_800621E0(XObj* obj);

// an XObj through the virtuals game/scr.h gives no arguments, the family calls them on what
// they carry and on themselves, see game/ccameraaction.h
class UnkCarried {
public:
	HSD_JObj* mJObj; // _00
	u8 _04[0x30];    // _04
	// _34 vtable

	virtual void vf08();
	virtual void vf0C();
	virtual void vf10();
	virtual void vf14();
	virtual void vf18();
	virtual void vf1C();
	virtual void vf20();
	virtual void vf24();
	virtual void vf28();
	virtual void vf2C();
	virtual void vf30();
	virtual void vf34();
	virtual void vf38();
	virtual void SetScale(f32 x, f32 y, f32 z, s32 force); // _3C
	virtual void vf40();
	virtual void vf44();
	virtual void vf48();
	virtual void vf4C();
	virtual void vf50();
	virtual void Release(); // _54
	virtual void vf58();
	virtual void vf5C();
	virtual void vf60();
	virtual void vf64();
	virtual void SetMoveTranslate(f32 x, f32 y, f32 z, s32 force); // _68
	virtual void vf6C();
	virtual void vf70();
	virtual void vf74();
	virtual void vf78();
	virtual void vf7C();
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate); // _80
	virtual void vf84();
	virtual void vf88();
	virtual void vf8C();
	virtual void vf90();
	virtual void vf94();
	virtual void vf98();
	virtual void vf9C();
	virtual void vfA0();
	virtual void vfA4();
	virtual void vfA8();
	virtual void vfAC();
	virtual void vfB0();
	virtual void vfB4();
	virtual void vfB8();
	virtual void vfBC();
	virtual void vfC0();
	virtual void vfC4();
	virtual void vfC8();
	virtual void vfCC();
	virtual void vfD0();
	virtual void vfD4();
	virtual void vfD8();
	virtual void vfDC();
	virtual void vfE0();
	virtual void vfE4();
	virtual void vfE8();
	virtual void vfEC();
	virtual void* GetAnimJoint(s32 id);      // _F0
	virtual void* GetMatAnimJoint(s32 id);   // _F4
	virtual void* GetShapeAnimJoint(s32 id); // _F8

	u8 _38[0x228 - 0x38]; // _38
	u64 mFlags;           // _228
};

// the robot, only what the family looks at
class UnkPlayer : public XObj {
public:
	u32 _980;                 // _980
	u8 _984[0x12DF4 - 0x984]; // _984
	s32 _12DF4;               // _12DF4
};
#define GAME_PLAYER (*(UnkPlayer**)(lbl_8025C400 + 0x96BC))

// takes the controls away from the player
static inline void LockPlayer(UnkPlayer* player)
{
	player->_980 = 0;
	player->_980 |= 0x40000000;
	player->_12DF4 = 0;
}

static inline u32 TestFlag(const CStatus& status, u32 mask)
{
	return status.mFlags & mask;
}

// more of the game object's pools, see game/scr.h
#define GAME_POOL_9734 ((UnkObjPool*)(lbl_8025C400 + 0x9734)) // 64 of them
#define GAME_POOL_975C ((UnkObjPool*)(lbl_8025C400 + 0x975C)) // 15 of them
#define GAME_POOL_97D4 ((UnkObjPool*)(lbl_8025C400 + 0x97D4)) // 21 of them
#define GAME_POOL_984C ((UnkObjPool*)(lbl_8025C400 + 0x984C)) // 288 of them

#define GAME_PAPA  (*(XObjActorPapa**)(lbl_8025C400 + 0x9798))
#define GAME_TAO   (*(XObjActorTao**)(lbl_8025C400 + 0x97AC))
#define GAME_FLAGS (*(u64*)(lbl_8025C400 + 0x1B10))

class XObjActorPapa;
class XObjActorTao;

// the kind 0x159 object's, game/scrd.cpp
extern "C" bool fn_8011566C(XObj* obj);

// the objects placed by id
struct UnkObjTable {
	void* mData; // _00
};
extern "C" XObj* fn_8007F570(UnkObjTable* table, s16 id);
#define GAME_OBJTABLE ((UnkObjTable*)(lbl_8025C400 + 0x5D080))

static inline XObj* FindObj(s16 id)
{
	UnkObjTable* table = GAME_OBJTABLE;
	return table->mData != NULL ? fn_8007F570(table, id) : NULL;
}

// name from __vt__13XObjActorMama
class XObjActorMama : public XObjActor {
public:
	typedef void (XObjActorMama::*State)(u32 frames);

	State mState; // _C78
	f32 _C84;     // _C84
	s32 _C88;     // _C88
	CStatus _C8C; // _C8C

	virtual ~XObjActorMama();               // _08
	virtual void Update(u32 frames);        // _A4
	virtual s32 vf124(s32 argc, s32* argv); // _124
	virtual void vf148(u32 frames);         // _148
	virtual void vf14C(u32 frames);         // _14C
	virtual void vf150();                   // _150
	virtual void vf158(u32 frames);         // _158
	virtual void vf15C();                   // _15C
	virtual HSD_JObj* vf168();              // _168
	virtual CVec vf16C();                   // _16C
	virtual f32 vf170();                    // _170
	virtual f32 vf174();                    // _174
	virtual f32 vf178();                    // _178
	virtual f32 vf17C();                    // _17C

	void StateNone(u32 frames);
	void Drop();
};
#if !defined(VERSION_JP)
typedef char XObjActorMama_size_check[sizeof(XObjActorMama) == 0xC98 ? 1 : -1];
#endif

// name from __vt__13XObjActorPapa
class XObjActorPapa : public XObjActor {
public:
	typedef void (XObjActorPapa::*State)(u32 frames);

	State mState; // _C78
	f32 _C84;     // _C84
	s32 _C88;     // _C88
	CStatus _C8C; // _C8C
	u8 _C94;      // _C94

	virtual ~XObjActorPapa();               // _08
	virtual s32 vf124(s32 argc, s32* argv); // _124
	virtual void vf148(u32 frames);         // _148
	virtual void vf14C(u32 frames);         // _14C
	virtual void vf150();                   // _150
	virtual void vf158(u32 frames);         // _158
	virtual void vf15C();                   // _15C
	virtual HSD_JObj* vf168();              // _168
	virtual CVec vf16C();                   // _16C
	virtual f32 vf178();                    // _178
	virtual f32 vf17C();                    // _17C

	bool IsFree();
	void StateNone(u32 frames);
	void StatePickUp(u32 frames);
	void Drop();
};
#if !defined(VERSION_JP)
typedef char XObjActorPapa_size_check[sizeof(XObjActorPapa) == 0xC98 ? 1 : -1];
#endif

// name from __vt__12XObjActorTao
class XObjActorTao : public XObjActor {
public:
	typedef void (XObjActorTao::*State)(u32 frames);

	State mState;           // _C78
	f32 _C84;               // _C84
	s32 _C88;               // _C88
	CStatus _C8C;           // _C8C
	Mtx _C94;               // _C94, where it holds what it carries
	u8 _CC4[0xCF4 - 0xCC4]; // _CC4
	s16 _CF4;               // _CF4
	s32 _CF8;               // _CF8
	XObj* _CFC;             // _CFC

	virtual ~XObjActorTao();                // _08
	virtual s32 vf124(s32 argc, s32* argv); // _124
	virtual void vf148(u32 frames);         // _148
	virtual void vf14C(u32 frames);         // _14C
	virtual void vf150();                   // _150
	virtual void vf158(u32 frames);         // _158
	virtual void vf15C();                   // _15C
	virtual HSD_JObj* vf168();              // _168
	virtual CVec vf16C();                   // _16C

	bool IsFree();
	void StateNone(u32 frames);
	void Drop();
	void StatePickUp(u32 frames);
};
#if !defined(VERSION_JP)
typedef char XObjActorTao_size_check[sizeof(XObjActorTao) == 0xD00 ? 1 : -1];
#endif

// name from __vt__13XObjActorDeca
class XObjActorDeca : public XObjActor {
public:
	typedef void (XObjActorDeca::*State)(u32 frames);

	State mState; // _C78
	f32 _C84;     // _C84
	f32 _C88;     // _C88
	s32 _C8C;     // _C8C
	CStatus _C90; // _C90
	CStatus _C98; // _C98
	f32 _CA0;     // _CA0
	f32 _CA4;     // _CA4

	virtual ~XObjActorDeca();               // _08
	virtual void UpdateAnim(u32 speed);     // _88
	virtual void Update(u32 frames);        // _A4
	virtual Vec GetCameraTarget();          // _EC
	virtual s32 vf124(s32 argc, s32* argv); // _124
	virtual void vf148(u32 frames);         // _148
	virtual void vf14C(u32 frames);         // _14C
	virtual void vf150();                   // _150
	virtual void vf158(u32 frames);         // _158
	virtual void vf15C();                   // _15C
	virtual HSD_JObj* vf168();              // _168
	virtual CVec vf16C();                   // _16C

	void StateHold(u32 frames);
	void StatePickUp(u32 frames);
};
#if !defined(VERSION_JP)
typedef char XObjActorDeca_size_check[sizeof(XObjActorDeca) == 0xCA8 ? 1 : -1];
#endif

#endif
