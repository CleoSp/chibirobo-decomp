#include "game/title.h"
#include "game/xobjs.h"
#include <math.h>
#include "game/gmath.h"

/**
 * @note Address: 0x80026B64
 * @note Size: 0x38
 */
// Cursor's vfAC
extern "C" void fn_80026B64(Cursor* cursor)
{
	if (cursor->mJObj != NULL) {
		HSD_JObjDispAll(cursor->mJObj, NULL, 7, 0);
	}
}

/**
 * @note Address: 0x80026B9C
 * @note Size: 0x90
 */
// Cursor's update, spins it
extern "C" void fn_80026B9C(Cursor* cursor, u32 frames)
{
	cursor->mAngle = fmod(cursor->mAngleSpeed * (f32)frames + cursor->mAngle, TWO_PI);
	cursor->SetRotationZ(cursor->mAngle, 0);
	fn_80090ED4(cursor, frames);
}

/**
 * @note Address: 0x80026C2C
 * @note Size: 0x4C
 */
extern "C" BOOL fn_80026C2C(Cursor* cursor, CoordInfo* info)
{
	if (fn_80091C80(cursor, info)) {
		HSD_JObjSetFlags(cursor->mJObj, CLASSICAL_SCALE);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80026C78
 * @note Size: 0x38
 */
extern "C" void fn_80026C78(Cursor* cursor, u32 id)
{
	fn_800929C4(cursor, id);
	cursor->mAngleSpeed = 0.0f;
	cursor->mAngle      = 0.0f;
}
