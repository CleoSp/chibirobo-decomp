#ifndef _GAME_FBEFFECT_H
#define _GAME_FBEFFECT_H

#include "Version.h"
#include "Dolphin/gx.h"

// frame buffer effects CGame owns, the outline at CGame + 0x4204 and the blur at CGame + 0x4268.
// class names from RTTI. no base class, both RTTI base lists are empty
// the vtables and the weak dtors are emitted in CGame's file

class CFbOutline {
public:
	void Init(u16 w, u16 h);
#if defined(VERSION_PAL)
	void SetSize(u16 w, u16 h);
#endif
	void Update(s32 frames);
	void Draw(u32 flags);

	// inlined:
	void Copy(GXTexFmt format);

	u16 mWidth;            // _00
	u16 mHeight;           // _02
	u8* mBuf;              // _04, w*h I8
	GXTexObj mTexObj;      // _08
	GXColor mColor0;       // _28, line color, alpha is the id for Draw 8
	GXColor mColor1;       // _2C, Draw 8 tint
	GXColor mColor2;       // _30, Draw 0x10
	GXColor mColor3;       // _34, Draw 0x20, skipped if alpha is 0
	GXColor mColor4;       // _38, Draw 0x80, set only by CGame
	f32 mThick;            // _3C
	f32 mThick2;           // _40, sign picks the blend
	GXTexObj* mPatternTex; // _44
	f32 mPatternTransX;    // _48
	f32 mPatternTransY;    // _4C
	f32 mPatternScaleX;    // _50
	f32 mPatternScaleY;    // _54
	s32 mFrame;            // _58, 0 to 3
	f32 mZoom;             // _5C
	virtual ~CFbOutline(); // _60 vtable
};

STATIC_ASSERT(sizeof(CFbOutline) == 0x64);

class CFbBlur {
public:
	void Init(u16 w, u16 h);
#if defined(VERSION_PAL)
	void SetSize(u16 w, u16 h);
#endif
	void Update(u32 frames);
#if defined(VERSION_PAL)
	void Draw();
	void Copy();
#else
	void Draw(BOOL half);
	void Copy(BOOL half);
#endif
	void DrawTex();

	u16 mWidth;         // _00
	u16 mHeight;        // _02
	void* mBuf;         // _04, w*h*2 RGB565
	GXTexObj mTexObj;   // _08
	f32 mAlpha;         // _28
	f32 mTarget;        // _2C
	f32 mSpeed;         // _30
	f32 mOffsetX;       // _34
	f32 mOffsetY;       // _38
	u8 mValid;          // _3C
	virtual ~CFbBlur(); // _40 vtable
};

STATIC_ASSERT(sizeof(CFbBlur) == 0x44);

#endif
