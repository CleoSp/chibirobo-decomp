#ifndef _GAME_CJOBJSET_H
#define _GAME_CJOBJSET_H

#include "game/cjobj.h"

// the three CJObj setters that build a Vec on the stack. the aggregate leaves a 0xC template
// in .rodata of every file that sees it and most files that know CJObj have none, so only
// the ones that emit or inline these include this. kept in this order, the templates then
// land in .rodata the way retail has them

#pragma push
#pragma pool_data on

inline void CJObj::SetRotation(f32 x, f32 y, f32 z, s32 force)
{
	if (force == 0) {
		Vec v = { x, y, z };
		// retail tests the cached translation here
		if (!(mTranslate != v)) {
			return;
		}
	}
	mRotate.x = x;
	mRotate.y = y;
	mRotate.z = z;
	UpdateRotation();
}

inline void CJObj::SetScale(f32 x, f32 y, f32 z, s32 force)
{
	if (force == 0) {
		Vec v = { x, y, z };
		if (!(mScale != v)) {
			return;
		}
	}
	mScale.x = x;
	mScale.y = y;
	mScale.z = z;
	HSD_JObjSetScale(mJObj, &mScale);
}

inline void CJObj::SetTranslate(f32 x, f32 y, f32 z, s32 force)
{
	if (force == 0) {
		Vec v = { x, y, z };
		if (!(mTranslate != v)) {
			return;
		}
	}
	mTranslate.x = x;
	mTranslate.y = y;
	mTranslate.z = z;
	HSD_JObjSetTranslate(mJObj, &mTranslate);
}

#pragma pop

#endif
