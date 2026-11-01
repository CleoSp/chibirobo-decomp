#include "game/actor_pool_shared.h"

CMsgQueue::~CMsgQueue()
{
}
XObjRead::~XObjRead()
{
}
XObjReadOne::~XObjReadOne()
{
}
XObjReadAll::~XObjReadAll()
{
}

struct ActorPool {
	u8* mData;
	s32 mUnknown;
	s32 mCount;
	const char* mName;
	s32 mStride;
	XObj* Get(s32 index) const { return (XObj*)(mData + index * mStride); }
};
extern "C" {
typedef void* ActorArrayDestructor(void*, s16);
void __destroy_arr(void*, ActorArrayDestructor*, u32, u32);
extern u8 lbl_804F41F0[];
void* fn_800DD5A0(void*, s16);
extern u8 lbl_804E1060[];
void* fn_800E239C(void*, s16);
extern u8 lbl_804B9850[];
void* fn_800E2240(void*, s16);
extern u8 lbl_804ADE48[];
void* __dt__15XObjSpiderSmallFv(void*, s16);
extern u8 lbl_804A06B8[];
void* __dt__10XObjSpiderFv(void*, s16);
extern u8 lbl_804924E8[];
void* __dt__8XObjArmyFv(void*, s16);
extern u8 lbl_80490B88[];
void* __dt__13XObjActorDecaFv(void*, s16);
extern u8 lbl_8046EF00[];
void* __dt__12XObjActorToyFv(void*, s16);
extern u8 lbl_80467330[];
void* __dt__12XObjLetickerFv(void*, s16);
extern u8 lbl_80441120[];
void* fn_800E24C0(void*, s16);
extern u8 lbl_80405710[];
void* fn_800E2460(void*, s16);
extern u8 lbl_80400A00[];
void* fn_800DD32C(void*, s16);
extern u8 lbl_8039CDF0[];
void* __dt__10XObjActor3Fv(void*, s16);
extern u8 lbl_8039BAE0[];
void* __dt__4XObjFv(void*, s16);
}
