#include "game/xobjattachment.h"

// todo: weak copy
/**
 * @note Address: 0x800DBC04
 * @note Size: 0x7C
 */
extern "C" XObjPlugCordPoint* fn_800DBC04(XObjPlugCordPoint* point, s32 flags)
{
	if (point != NULL) {
		point->mSpeed.CVec::~CVec();
		point->mTarget.CVec::~CVec();
		point->mPos.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(point);
	}
	return point;
}
