#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "sdk/msl/c/cstddef"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__10CFbOutline[];
extern void* __vt__10CScrnColor[];
extern void* __vt__10CScrnQuake[];
extern void* __vt__12CFileArchive[];
extern void* __vt__12CScrnWipeTex[];
extern void* __vt__13CScrnRoboeyes[];
extern void* __vt__13CScrnWipeTile[];
extern void* __vt__14CCameraMemento[];
extern void* __vt__15CScrnWipeCircle[];
extern void* __vt__15CScrnWipeCustom[];
extern void* __vt__20CMotionControlCamera[];
extern void* __vt__4CEvt[];
extern void* __vt__4CTpl[];
extern void* __vt__4CVec[];
extern void* __vt__4Logo[];
extern void* __vt__5CCard[];
extern void* __vt__5CFile[];
extern void* __vt__5CGame[];
extern void* __vt__5XOBJS[];
extern void* __vt__6CMovie[];
extern void* __vt__6CSound[];
extern void* __vt__6CStack[];
extern void* __vt__6XSound[];
extern void* __vt__7CCamera[];
extern void* __vt__7CFbBlur[];
extern void* __vt__7CStatus[];
extern void* __vt__7CTexObj[];
extern void* __vt__7Memcard[];
extern void* __vt__8CEvtData[];
extern void* __vt__8FbSubwin[];
extern void* __vt__8InfoScrn[];
extern void* __vt__9CFbTexEff[];
extern void* __vt__9CParticle[];
extern void* __vt__9CScrnFade[];
extern void* __vt__9CScrnWipe[];
CBase* __ct__5CBaseFv(CBase* base);
void* fn_80019E5C(void*);
void* __ct__4CMsgFv(void*);
void* fn_8001B2EC(void*);
void* fn_8001B32C(void*);
void* fn_8001B33C(void*);
void* fn_8001B35C(void*);
void* fn_8001B38C(void*);
void* fn_8001B408(void*);
void* __dt__5CFileFv(void*, s32);
void* dtor_80008DC8(void*, s32);
void* fn_80008D80(void*, s32);
void* fn_80008E10(void*, s32);
void* __dt__4CMsgFv(void*, s32);
}

// todo: this is CGame::CGame() once cgame.h's members get types,
// for now the stores go off each object's member
/**
 * @note Address: 0x800197A4
 * @note Size: 0x6B8
 */
extern "C" CGame* __ct__5CGameFv(CGame* game)
{
	u8* bytes = (u8*)game;

	__ct__5CBaseFv(game);
	*(void***)(bytes + offsetof(CGame, mReset) + 0x4) = __vt__5CGame; // _3334 vtable, _3318 in japan

	// mArchive, a CFileArchive
	*(void***)(bytes + offsetof(CGame, mArchive) + 0xE8) = __vt__5CFile;
	*(u32*)(bytes + offsetof(CGame, mArchive) + 0xD4)    = 0;
	*(s32*)(bytes + offsetof(CGame, mArchive) + 0xC0)    = -1;
	*(u32*)(bytes + offsetof(CGame, mArchive) + 0xCC)    = 0;
	*(u32*)(bytes + offsetof(CGame, mArchive) + 0xD0)    = 0;
	*(u32*)(bytes + offsetof(CGame, mArchive) + 0xE4)    = 1;
	*(void***)(bytes + offsetof(CGame, mArchive) + 0xE8) = __vt__12CFileArchive;

	// mTpl, the screen fades and wipes, the frame buffer effects and the logos
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xE8)  = __vt__5CFile;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xD4)     = 0;
	*(s32*)(bytes + offsetof(CGame, mTpl) + 0xC0)     = -1;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xCC)     = 0;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xD0)     = 0;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xE4)     = 1;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xE8)  = __vt__4CTpl;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x1E8) = __vt__7CTexObj;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x220) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x1F0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x1F4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x1F8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x1FC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x200)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x204)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x208)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x20C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x210)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x214)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x218)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x21C)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x220) = __vt__9CScrnFade;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x254) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x224)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x228)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x22C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x230)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x234)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x238)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x23C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x240)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x244)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x248)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x24C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x250)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x254) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x254) = __vt__15CScrnWipeCircle;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x2B8) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x288)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x28C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x290)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x294)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x298)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x29C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2A0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2A4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2A8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2AC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2B0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x2B4)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x2B8) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x2B8) = __vt__12CScrnWipeTex;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x3E8) = __vt__7CTexObj;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x41C) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x3EC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x3F0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x3F4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x3F8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x3FC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x400)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x404)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x408)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x40C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x410)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x414)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x418)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x41C) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x41C) = __vt__15CScrnWipeCustom;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x480) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x450)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x454)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x458)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x45C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x460)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x464)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x468)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x46C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x470)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x474)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x478)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x47C)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x480) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x480) = __vt__13CScrnWipeTile;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x4E4) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4B4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4B8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4BC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4C0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4C4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4C8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4CC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4D0)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4D4)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4D8)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4DC)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x4E0)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x4E4) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x4E4) = __vt__13CScrnWipeTile;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x548) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x518)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x51C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x520)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x524)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x528)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x52C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x530)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x534)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x538)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x53C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x540)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x544)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x548) = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x548) = __vt__13CScrnRoboeyes;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x5E4) = __vt__10CFbOutline;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x628) = __vt__7CFbBlur;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x74C) = __vt__9CFbTexEff;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x744) = __vt__7CTexObj;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x74C) = __vt__8FbSubwin;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x758) = __vt__7CStatus;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0x754)    = 0;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x900) = __vt__7CCamera;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x810) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x820) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x830) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x840) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x894) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0x938) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x908)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x90C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x910)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x914)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x918)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x91C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x920)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x924)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x928)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x92C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x930)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0x934)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xA38) = __vt__7CTexObj;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xA6C) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA3C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA40)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA44)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA48)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA4C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA50)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA54)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA58)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA5C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA60)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA64)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA68)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xAA0) = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA70)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA74)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA78)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA7C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA80)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA84)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA88)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA8C)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA90)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA94)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA98)    = 0.0f;
	*(f32*)(bytes + offsetof(CGame, mTpl) + 0xA9C)    = 0.0f;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xCAC) = __vt__4Logo;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xB98) = __vt__5CFile;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xB84)    = 0;
	*(s32*)(bytes + offsetof(CGame, mTpl) + 0xB70)    = -1;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xB7C)    = 0;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xB80)    = 0;
	*(u32*)(bytes + offsetof(CGame, mTpl) + 0xB94)    = 1;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xB98) = __vt__4CTpl;
	*(void***)(bytes + offsetof(CGame, mTpl) + 0xC98) = __vt__7CTexObj;

	// the title and the menu cursor
	fn_8001B408(bytes + offsetof(CGame, mTpl) + 0xCB0);
	fn_8001B38C(bytes + offsetof(CGame, _8580) + 0x0);

	// the info screen, particles, quake, XOBJS, the object archives, the event and messages
	*(void***)(bytes + offsetof(CGame, _8580) + 0xB90)  = __vt__8InfoScrn;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x9E8)  = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9B8)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9BC)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9C0)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9C4)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9C8)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9CC)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9D0)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9D4)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9D8)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9DC)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9E0)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9E4)     = 0.0f;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x9E8)  = __vt__9CScrnFade;
	*(void***)(bytes + offsetof(CGame, _8580) + 0xA1C)  = __vt__10CScrnColor;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9EC)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9F0)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9F4)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9F8)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0x9FC)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA00)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA04)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA08)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA0C)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA10)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA14)     = 0.0f;
	*(f32*)(bytes + offsetof(CGame, _8580) + 0xA18)     = 0.0f;
	*(void***)(bytes + offsetof(CGame, _8580) + 0xA1C)  = __vt__9CScrnWipe;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x10D8) = __vt__9CParticle;
	__construct_array(bytes + offsetof(CGame, _8580) + 0xB98, (ConstructorDestructor)fn_8001B35C, (ConstructorDestructor)__dt__5CFileFv,
	                  0xEC, 4);
	__construct_array(bytes + offsetof(CGame, _8580) + 0xF58, (ConstructorDestructor)fn_8001B33C, (ConstructorDestructor)fn_80008D80, 0xC,
	                  32);
	*(void***)(bytes + offsetof(CGame, _8580) + 0x1110) = __vt__10CScrnQuake;
	__construct_array(bytes + offsetof(CGame, _8580) + 0x10DC, (ConstructorDestructor)fn_8001B32C, (ConstructorDestructor)dtor_80008DC8,
	                  0x10, 3);
	*(void***)(bytes + offsetof(CGame, _8580) + 0x22EC) = __vt__5XOBJS;
	__construct_array(bytes + offsetof(CGame, _8580) + 0x6AFC, (ConstructorDestructor)fn_8001B2EC, (ConstructorDestructor)fn_80008E10,
	                  0x138, 1024);
	*(void***)(bytes + offsetof(CGame, _8580) + 0x54B60) = __vt__4CEvt;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x54B08) = __vt__8CEvtData;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B00)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B04)    = 0;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x54B2C) = __vt__6CStack;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B20)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B28)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B24)    = 0;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x54B3C) = __vt__6CStack;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B30)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B38)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B34)    = 0;
	*(void***)(bytes + offsetof(CGame, _8580) + 0x54B4C) = __vt__6CStack;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B40)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B48)    = 0;
	*(u32*)(bytes + offsetof(CGame, _8580) + 0x54B44)    = 0;
	__construct_array(bytes + offsetof(CGame, _8580) + 0x54B68, (ConstructorDestructor)__ct__4CMsgFv, (ConstructorDestructor)__dt__4CMsgFv,
	                  sizeof(game->mEvtData[0]), 2);

	// the camera and its actions
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x204) = __vt__20CMotionControlCamera;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x1A0) = __vt__7CCamera;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0xB0)  = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0xC0)  = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0xD0)  = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0xE0)  = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x134) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x1B0) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x1C0) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x1D0) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x1F8) = __vt__7CStatus;
	*(u32*)(bytes + offsetof(CGame, mTimeState) + 0x1F4)    = 0;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x214) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x224) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x234) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x240) = __vt__14CCameraMemento;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x250) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x260) = __vt__4CVec;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x270) = __vt__4CVec;
	fn_80019E5C(bytes + offsetof(CGame, mTimeState) + 0x284);

	// sound, memory card and movie
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x20FC) = __vt__6CSound;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x20FC) = __vt__6XSound;
	*(void***)(bytes + offsetof(CGame, mTimeState) + 0x21EC) = __vt__5CFile;
	*(u32*)(bytes + offsetof(CGame, mTimeState) + 0x21D8)    = 0;
	*(s32*)(bytes + offsetof(CGame, mTimeState) + 0x21C4)    = -1;
	*(u32*)(bytes + offsetof(CGame, mTimeState) + 0x21D0)    = 0;
	*(u32*)(bytes + offsetof(CGame, mTimeState) + 0x21D4)    = 0;
	*(u32*)(bytes + offsetof(CGame, mTimeState) + 0x21E8)    = 1;
	*(void***)(bytes + offsetof(CGame, mMemcard) + 0x16A20)  = __vt__7Memcard;
	*(void***)(bytes + offsetof(CGame, mMemcard) + 0xA040)   = __vt__5CCard;
	*(void***)(bytes + offsetof(CGame, mMemcard) + 0x17058)  = __vt__6CMovie;
	return game;
}
