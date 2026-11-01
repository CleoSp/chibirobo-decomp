#ifndef _GAME_CSCRN_H
#define _GAME_CSCRN_H

#include "game/ctpl.h"
#include "game/ut.h"

class CGs;

// full screen color, fade and wipe effects, CGame holds one of each. class names from
// RTTI, layouts are partial

// a color in floats so it can step a fraction per frame
struct ScrnColorF {
	f32 r; // _00
	f32 g; // _04
	f32 b; // _08
	f32 a; // _0C
};

// a flat color over the whole screen that fades to its target
class CScrnColor {
public:
	ScrnColorF mColor;  // _00
	ScrnColorF mTarget; // _10
	ScrnColorF mSpeed;  // _20, per frame, all zero once it's done
	// _30 vtable

	virtual ~CScrnColor() { } // _08 (weak)

	CScrnColor();
};
typedef char CScrnColor_size_check[sizeof(CScrnColor) == 0x34 ? 1 : -1];

class CScrnFade : public CScrnColor {
public:
	virtual ~CScrnFade() { } // _08 (weak)
	virtual void Reset();    // _0C, fn_8001357C
};

// a color with a hole that opens or closes, the subclasses pick the shape
class CScrnWipe : public CScrnColor {
public:
	virtual ~CScrnWipe() { }              // _08 (weak)
	virtual void Reset();                 // _0C, fn_800238A4
	virtual void Update(u32 frames);      // _10, fn_80023030
	virtual void Draw(CGs* gs, s32 mode); // _14, fn_8002289C

	BOOL mActive;       // _34
	BOOL mHideWhenDone; // _38, turns itself off once the size stops
	f32 mX;             // _3C
	f32 mY;             // _40
	f32 mWidth;         // _44
	f32 mHeight;        // _48
	f32 mTargetWidth;   // _4C
	f32 mTargetHeight;  // _50
	f32 mSpeedWidth;    // _54, zero once the wipe is done
	f32 mSpeedHeight;   // _58
	f32 _5C;            // _5C
	f32 _60;            // _60
};
typedef char CScrnWipe_size_check[sizeof(CScrnWipe) == 0x64 ? 1 : -1];

class CScrnWipeCircle : public CScrnWipe {
public:
	virtual ~CScrnWipeCircle() { } // _08 (weak)
};

class CScrnWipeTex : public CScrnWipe {
public:
	virtual ~CScrnWipeTex() { } // _08 (weak)

	CTexObj mTex; // _64
};
typedef char CScrnWipeTex_size_check[sizeof(CScrnWipeTex) == 0x164 ? 1 : -1];

class CScrnWipeCustom : public CScrnWipe {
public:
	virtual ~CScrnWipeCustom() { } // _08 (weak)
};

class CScrnWipeTile : public CScrnWipe {
public:
	virtual ~CScrnWipeTile() { } // _08 (weak)
};

// chibi robo's eyes closing over the screen
class CScrnRoboeyes : public CScrnWipe {
public:
	virtual ~CScrnRoboeyes() { }          // _08 (weak)
	virtual void Reset();                 // _0C, fn_80021500
	virtual void Update(u32 frames);      // _10, fn_80021474
	virtual void Draw(CGs* gs, s32 mode); // _14, fn_800207C8

	f32 mAngle; // _64
	f32 mPhase; // _68
};
typedef char CScrnRoboeyes_size_check[sizeof(CScrnRoboeyes) == 0x6C ? 1 : -1];

// shakes the screen, the amount steps to the target and flips sign every frame
class CScrnQuake {
public:
	CVec mAmount; // _00
	CVec mTarget; // _10
	CVec mSpeed;  // _20
	s32 mSign;    // _30
	// _34 vtable

	virtual ~CScrnQuake() { } // _08 (weak)
};
typedef char CScrnQuake_size_check[sizeof(CScrnQuake) == 0x38 ? 1 : -1];

extern "C" {
// CScrnColor
void fn_8002390C(CScrnColor* color, CGs* gs);
void fn_80023B38(CScrnColor* color, u32 frames);
void fn_80023D74(CScrnColor* color, u8 r, u8 g, u8 b, u8 a, f32 time);
void fn_80023EE0(CScrnColor* color, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1, f32 time);
// CScrnWipe
void fn_80023030(CScrnWipe* wipe, u32 frames);
void fn_80023438(CScrnWipe* wipe, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, f32 x, f32 y, f32 width0, f32 height0, f32 width1,
                 f32 height1, f32 time, u8 a1, BOOL hideWhenDone);
void fn_800236EC(CScrnWipe* wipe, u8 r, u8 g, u8 b, u8 a, BOOL hideWhenDone, f32 x, f32 y, f32 width0, f32 height0, f32 width1, f32 height1,
                 f32 time);
void fn_800238A4(CScrnWipe* wipe);
// CScrnRoboeyes
void fn_80021474(CScrnRoboeyes* eyes, u32 frames);
void fn_80021500(CScrnRoboeyes* eyes);
// CScrnQuake
void fn_800240EC(CScrnQuake* quake, u32 frames);
void fn_800242A8(CScrnQuake* quake, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 time);
void fn_80024524(CScrnQuake* quake);
}

#endif
