#ifndef _GAME_CGAME_H
#define _GAME_CGAME_H

#include "types.h"
#include "Version.h"
#include "game/xsound.h"

class XObj;
class CInterfaceCameraAction;

// the main game object, its one instance is lbl_8025C400. the ctor builds members up to at
// least +0xFF458, way past dtk's 0xA6C30 symbol size, so the tail here is still inside it
// japan is 0x48 shorter before +0x8F38 and 0xCE8 shorter before +0xE5D28, europe is 0x8 longer
// around +0x3330 and matches usa for every other field here
class CLight;
class CGame {
public:
	XObj* GetObject(s32 index) const { return mObjects[index]; }

#if defined(VERSION_JP)
	u8 _00000[0x3314];         // _00000
	s32 _3330;                 // _3330
	u8 _3334[0x8EF0 - 0x3318]; // _3334
#elif defined(VERSION_PAL)
	u8 _00000[0x3338];         // _00000
	s32 _3330;                 // _3330
	u8 _3334[0x8F38 - 0x333C]; // _3334
#else
	u8 _00000[0x3330];         // _00000
	s32 _3330;                 // _3330, _3314 in japan, _3338 in europe, stops fn_80132950's read loop when set
	u8 _3334[0x8F38 - 0x3334]; // _3334
#endif
	u8 _8F38[0x9114 - 0x8F38];  // _8F38, _8EF0 in japan
	u8 _9114[0x548];            // _9114, _90CC in japan, a CParticle, see game/cparticle.h
	u8 _965C[0x96BC - 0x965C];  // _965C
	XObj* mPlayer;              // _96BC, _9674 in japan, chibi robo's object
	u8 _96C0[0x9860 - 0x96C0];  // _96C0
	XObj* mObjects[0x400];      // _9860, _9818 in japan, the object table
	s32 mObjectCount;           // _A860, _A818 in japan, its count
	u8 _A864[0x5D080 - 0xA864]; // _A864
	void* _5D080;               // _5D080, _5D038 in japan
#if defined(VERSION_JP)
	u8 _5D084[0xE5040 - 0x5D03C]; // _5D084
#else
	u8 _5D084[0xE5D28 - 0x5D084]; // _5D084
#endif
	s32 _E5D28;                         // _E5D28, _E5040 in japan
	f32 _E5D2C;                         // _E5D2C, _E5044 in japan
	u8 _E5D30[0xE5D38 - 0xE5D30];       // _E5D30
	u8 _E5D38[0x274];                   // _E5D38, _E5050 in japan, the game's CMotionControlCamera, see game/cmotioncontrolcamera.h
	u8 _E5FAC[0x1E40];                  // _E5FAC, _E52C4 in japan, the CCameraActions the camera picks from
	CInterfaceCameraAction* _E7DEC[12]; // _E7DEC, _E7104 in japan, CCameraActions's table, see CAMACTION
	u8 _E7E1C[0xE7E20 - 0xE7E1C];       // _E7E1C, CCameraActions's vtable
	CLight* _E7E20;                     // _E7E20, _E7138 in japan, head of the light list, LIGHT walks it
	XSound _E7E24;                      // _E7E24, _E713C in japan
};

#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(CGame) == 0xE73F8);
#else
STATIC_ASSERT(sizeof(CGame) == 0xE80E0);
#endif

extern "C" CGame lbl_8025C400;

#endif
