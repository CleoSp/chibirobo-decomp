#ifndef _GAME_XOBJACTORBASE_H
#define _GAME_XOBJACTORBASE_H

#include "game/scr.h"
extern "C" {
#include <dolphin/mtx/mtx.h>
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
}

// XObjActor, the abstract XObj the stage characters build on. game/xobjactor.h has it on
// game/xobj.h's XObj, this copy sits on game/scr.h's so the actors here can derive from it

// one of the actor's 8 slots, 0x38 bytes
struct UnkActorSlot {
	s32 _00;      // _00
	s32 _04;      // _04
	u8 _08[0x30]; // _08

	void Clear()
	{
		_04 = 0;
		_00 = 0;
	}
};

class XObjActor : public XObj {
public:
	f32 _980;               // _980
	f32 _984;               // _984
	f32 _988;               // _988
	f32 _98C;               // _98C
	s32 _990;               // _990
	s32 _994;               // _994
	CStatus mStatus;        // _998
	s32 _9A0;               // _9A0
	s32 _9A4;               // _9A4
	f32 _9A8;               // _9A8
	f32 _9AC;               // _9AC
	Vec _9B0;               // _9B0
	Vec _9BC;               // _9BC
	Vec _9C8;               // _9C8
	f32 _9D4;               // _9D4
	f32 _9D8;               // _9D8
	f32 _9DC;               // _9DC
	s32 _9E0;               // _9E0
	s32 _9E4;               // _9E4
	s32 _9E8[16];           // _9E8
	s32 _A28;               // _A28
	s32 _A2C;               // _A2C
	u8 _A30[0x8];           // _A30
	f32 _A38;               // _A38
	f32 _A3C;               // _A3C
	Vec _A40;               // _A40
	s16 _A4C;               // _A4C
	XObj* _A50;             // _A50
	u8 _A54[0xA84 - 0xA54]; // _A54
	XObj* _A84;             // _A84
	s32 _A88;               // _A88
	UnkActorSe mSe;         // _A8C
	s32 _A9C;               // _A9C
	f32 _AA0;               // _AA0
	f32 _AA4;               // _AA4
	f32 _AA8;               // _AA8
	s32 _AAC;               // _AAC
	f32 _AB0;               // _AB0
	UnkActorSlot mSlots[8]; // _AB4

	virtual ~XObjActor(); // _08, 0x80106A14

	virtual void vf148(u32 frames);     // _148
	virtual void vf14C(u32 frames) = 0; // _14C
	virtual void vf150()           = 0; // _150
	virtual void vf154(u32 frames);     // _154
	virtual void vf158(u32 frames) = 0; // _158
	virtual void vf15C();               // _15C
	virtual u32 vf160();                // _160
	virtual void vf164(f32 frame);      // _164
	virtual HSD_JObj* vf168();          // _168
	virtual CVec vf16C();               // _16C
	virtual f32 vf170();                // _170
	virtual f32 vf174();                // _174
	virtual f32 vf178();                // _178
	virtual f32 vf17C();                // _17C
};
// 0xC74 rounded up to XObj's u64 alignment
typedef char XObjActor_size_check[sizeof(XObjActor) == 0xC78 ? 1 : -1];

// more jobj.h inlines, the assert lines are from the binary
inline f32 HSD_JObjGetScaleX(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(899, jobj);
	return jobj->scale.x;
}

inline f32 HSD_JObjGetScaleY(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(911, jobj);
	return jobj->scale.y;
}

inline f32 HSD_JObjGetScaleZ(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(923, jobj);
	return jobj->scale.z;
}

// see game/objb.h, hands the matrix back to the srt values
inline void HSD_JObjClearUserMtx(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1199, jobj);
	jobj->flags &= ~0x03800000;
	HSD_JObjSetMtxDirty(jobj);
}

// GetJointByPath with the path walked by pointer
inline HSD_JObj* GetJointFromRoot(HSD_JObj* jobj, s32 depth, s32* path)
{
	if (jobj == NULL) {
		return NULL;
	}
	for (s32 i = 0; i < depth; i++) {
		jobj = HSD_JObjGetChild(jobj);
		for (s32 j = 0; j < *path; j++) {
			if ((jobj = HSD_JObjGetNext(jobj)) == NULL) {
				return NULL;
			}
		}
		path++;
	}
	return jobj;
}

extern "C" void HSD_MtxGetTranslate(Mtx mat, Vec* vec);

// the jobj's place in the world, see game/obja.h
inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// the XObjActor methods the actors call directly, game/xobjactorupdate.cpp and around
extern "C" {
void fn_80106BE0(XObjActor* self, f32 frame);          // vf164
void fn_8010706C(XObjActor* self);                     // clears _990, _994 and _984
s32 fn_801070A4(XObjActor* self, s32 argc, s32* argv); // vf124
void fn_80107600(XObjActor* self);                     // vfC8
void fn_80107620(XObjActor* self);                     // vfAC
void fn_801076DC(XObjActor* self, f32 a, f32 b);
void fn_8010771C(XObjActor* self, f32 rate);
void fn_80107750(XObjActor* self, u32 frames); // vf148
BOOL fn_80107F18(XObjActor* self, void* info); // vf9C
void fn_80108014(XObjActor* self);             // vf94
void fn_801081F0(XObjActor* self);             // Reset
void fn_801083F0(XObjActor* self, u32 id);     // Init
}

#endif
