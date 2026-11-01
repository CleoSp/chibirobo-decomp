#include "game/xchr.h"
#include "game/evt.h"
#include <math.h>

struct XObjChrCameraPath {
	s32 mSteps[8]; // _00
};

extern "C" {
extern const XObjChrCameraPath lbl_80209DB8;
extern const XObjChrCameraPath lbl_80209DD8;
}

// halfway between two of his joints, pulled back a little along his heading
/**
 * @note Address: 0x800A09BC
 * @note Size: 0x21C
 */
extern "C" Vec fn_800A09BC(XObjChr* chr)
{
	XObjChrCameraPath firstPath  = lbl_80209DB8;
	CVec first                   = fn_8007E7F4(chr, 8, firstPath.mSteps);
	XObjChrCameraPath secondPath = lbl_80209DD8;
	CVec second                  = fn_8007E7F4(chr, 8, secondPath.mSteps);
	Vec point                    = first + (Vec)((second - (Vec)first) / 2.0f);
	point.x -= 1.5f * (f32)sin(chr->mMoveRotate.y);
	point.z -= 1.5f * (f32)cos(chr->mMoveRotate.y);
	return point;
}
