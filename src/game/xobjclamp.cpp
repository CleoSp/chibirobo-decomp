#include "game/xobj.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
void PSMTXRotRad(Mtx matrix, char axis, f32 angle);
}

/**
 * @note Address: 0x8008ADA0
 * @note Size: 0x338
 */
extern "C" BOOL fn_8008ADA0(XObj* object, const Vec* direction, u8 axes, f32 x, f32 z)
{
	BOOL changed = FALSE;
	if (__fpclassifyf(object->mAnchorX) == FP_NAN || __fpclassifyf(object->mAnchorZ) == FP_NAN) {
		object->SetTranslateX(x, 0);
		object->SetTranslateZ(z, 0);
	} else if (object->mAnchorX == x && object->mAnchorZ == z) {
		object->SetTranslateX(x, 0);
		object->SetTranslateZ(z, 0);
	} else {
		Mtx rotation;
		Vec displacement;
		PSMTXRotRad(rotation, 'y', -object->mMoveRotate.y);
		displacement.x = object->mAnchorX - x;
		displacement.z = object->mAnchorZ - z;
		displacement.y = 0.0f;
		PSMTXMultVec(rotation, &displacement, &displacement);

		if (axes & 1) {
			if (direction->x > 0.0f && displacement.x < object->mLimitPosX) {
				displacement.x = object->mLimitPosX;
				changed        = TRUE;
			} else if (direction->x < 0.0f && displacement.x > object->mLimitNegX) {
				displacement.x = object->mLimitNegX;
				changed        = TRUE;
			}
		}
		if (axes & 2) {
			if (direction->z > 0.0f && displacement.z < object->mLimitPosZ) {
				displacement.z = object->mLimitPosZ;
				changed        = TRUE;
			} else if (direction->z < 0.0f && displacement.z > object->mLimitNegZ) {
				displacement.z = object->mLimitNegZ;
				changed        = TRUE;
			}
		}

		PSMTXRotRad(rotation, 'y', object->mMoveRotate.y);
		PSMTXMultVec(rotation, &displacement, &displacement);
		object->SetTranslateX(object->mAnchorX - displacement.x, 0);
		object->SetTranslateZ(object->mAnchorZ - displacement.z, 0);
	}
	return changed;
}
