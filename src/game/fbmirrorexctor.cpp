#include "types.h"

class FbMirrorEx;

extern "C" {
extern void* __vt__9CFbTexEff[];
extern void* __vt__7CTexObj[];
extern void* __vt__8FbMirror[];
extern void* __vt__7CCamera[];
extern void* __vt__4CVec[];
extern void* __vt__10FbMirrorEx[];
}

// todo: weak copy
/**
 * @note Address: 0x80019700
 * @note Size: 0x5C
 */
extern "C" FbMirrorEx* fn_80019700(FbMirrorEx* mirror)
{
	u8* bytes                 = (u8*)mirror;
	*(void***)(bytes + 0x120) = __vt__9CFbTexEff;
	*(void***)(bytes + 0x118) = __vt__7CTexObj; // mTex's
	*(void***)(bytes + 0x120) = __vt__8FbMirror;
	*(void***)(bytes + 0x2B8) = __vt__7CCamera; // mCamera's
	*(void***)(bytes + 0x1C8) = __vt__4CVec;    // and its four CVecs
	*(void***)(bytes + 0x1D8) = __vt__4CVec;
	*(void***)(bytes + 0x1E8) = __vt__4CVec;
	*(void***)(bytes + 0x1F8) = __vt__4CVec;
	*(void***)(bytes + 0x24C) = __vt__4CVec;
	*(void***)(bytes + 0x120) = __vt__10FbMirrorEx;
	return mirror;
}
