#ifndef _GAME_FBMIRROR_H
#define _GAME_FBMIRROR_H

#include "game/ccamera.h"
#include "game/ctpl.h"

// frame buffer texture effects, the mirrors render the scene again from their own camera.
// class names from RTTI, layouts are partial. no dtor in the vtables

class CFbTexEff {
public:
	u8 _00[0x1C]; // _00
	CTexObj mTex; // _1C
	u32 _11C;     // _11C
	// _120 vtable

	~CFbTexEff() { }

	virtual void vf08(); // _08
	virtual void vf0C(); // _0C
	virtual void vf10(); // _10
	virtual void vf14(); // _14
	virtual void vf18(); // _18
};
typedef char CFbTexEff_size_check[sizeof(CFbTexEff) == 0x124 ? 1 : -1];

class FbMirror : public CFbTexEff {
public:
	u32 _124;        // _124
	CCamera mCamera; // _128

	~FbMirror() { }
};
typedef char FbMirror_size_check[sizeof(FbMirror) == 0x2BC ? 1 : -1];

class FbMirrorEx : public FbMirror {
public:
	virtual void vf08(); // _08, fn_8001BC34

	u8 _2BC[0x2F0 - 0x2BC]; // _2BC
	u32 _2F0;               // _2F0
};
typedef char FbMirrorEx_size_check[sizeof(FbMirrorEx) == 0x2F4 ? 1 : -1];

#endif
