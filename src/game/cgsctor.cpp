#include "game/hsd_jobj.h"
#include "game/cgs.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__3CGs[];
extern void* __vt__9CGsScreen[];
extern void* __vt__13CGsRenderMode[];
extern void* __vt__5CFont[];
extern void* __vt__7CTexObj[];
extern void* __vt__10CPrimitive[];
extern void* __vt__7CSprite[];
extern void* __vt__5CLine[];
extern void* __vt__9CTriangle[];
extern void* __vt__7CCircle[];
extern void* __vt__5CTile[];
extern void* __vt__5CCube[];
extern void* __vt__11COctahedron[];
extern void* __vt__7CDodeca[];
extern void* __vt__12CIcosahedron[];
extern void* __vt__7CSphere[];
extern void* __vt__9CCylinder[];
extern void* __vt__6CTorus[];
// the weak ctors and dtors of CGs's arrays
CGsTevStage* fn_8001BC24(CGsTevStage* stage);
CGsTevStage* fn_80008344(CGsTevStage* stage, s32 flags);
CGsChannel* fn_8001BC14(CGsChannel* channel);
CGsChannel* fn_8000838C(CGsChannel* channel, s32 flags);
}

/**
 * @note Address: 0x8001B9B8
 * @note Size: 0x1A4
 */
// todo: weak copy
extern "C" CGs* fn_8001B9B8(CGs* gs)
{
	*(void***)((u8*)gs + sizeof(CGs) - 4)                 = __vt__3CGs;
	*(void***)((u8*)&gs->mScreen + sizeof(CGsScreen) - 4) = __vt__9CGsScreen;
	gs->mScreen.mMode                                     = -1;
	*(void***)((u8*)&gs->mRenderMode + 0x30)              = __vt__13CGsRenderMode;
	__construct_array(gs->mTevStages, (ConstructorDestructor)fn_8001BC24, (ConstructorDestructor)fn_80008344, sizeof(CGsTevStage), 2);
	__construct_array(gs->mChannels, (ConstructorDestructor)fn_8001BC14, (ConstructorDestructor)fn_8000838C, sizeof(CGsChannel), 4);
	*(void***)((u8*)&gs->mFont + 0x13C)        = __vt__5CFont;
	*(void***)((u8*)&gs->mFont.mTex + 0xFC)    = __vt__7CTexObj;
	gs->mFont.mDesc.textureHeader              = NULL;
	*(void***)((u8*)&gs->mTex + 0xFC)          = __vt__7CTexObj;
	*(void***)((u8*)&gs->mSprite + 0x164)      = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mSprite + 0x164)      = __vt__7CSprite;
	*(void***)((u8*)&gs->mLine + 0x164)        = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mLine + 0x164)        = __vt__5CLine;
	*(void***)((u8*)&gs->mTriangle + 0x164)    = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mTriangle + 0x164)    = __vt__9CTriangle;
	*(void***)((u8*)&gs->mCircle + 0x164)      = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mCircle + 0x164)      = __vt__7CCircle;
	*(void***)((u8*)&gs->mTile + 0x164)        = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mTile + 0x164)        = __vt__5CTile;
	*(void***)((u8*)&gs->mSprite2 + 0x164)     = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mSprite2 + 0x164)     = __vt__7CSprite;
	*(void***)((u8*)&gs->mCube + 0x164)        = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mCube + 0x164)        = __vt__5CCube;
	*(void***)((u8*)&gs->mOctahedron + 0x164)  = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mOctahedron + 0x164)  = __vt__11COctahedron;
	*(void***)((u8*)&gs->mDodeca + 0x164)      = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mDodeca + 0x164)      = __vt__7CDodeca;
	*(void***)((u8*)&gs->mIcosahedron + 0x164) = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mIcosahedron + 0x164) = __vt__12CIcosahedron;
	*(void***)((u8*)&gs->mSphere + 0x164)      = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mSphere + 0x164)      = __vt__7CSphere;
	*(void***)((u8*)&gs->mCylinder + 0x164)    = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mCylinder + 0x164)    = __vt__9CCylinder;
	*(void***)((u8*)&gs->mTorus + 0x164)       = __vt__10CPrimitive;
	*(void***)((u8*)&gs->mTorus + 0x164)       = __vt__6CTorus;
	return gs;
}
