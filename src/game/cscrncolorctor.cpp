#include "game/hsd_jobj.h"
#include "game/cscrn.h"

/**
 * @note Address: 0x80134C20
 * @note Size: 0x44
 */
CScrnColor::CScrnColor()
{
	mColor.r  = 0.0f;
	mColor.g  = 0.0f;
	mColor.b  = 0.0f;
	mColor.a  = 0.0f;
	mTarget.r = 0.0f;
	mTarget.g = 0.0f;
	mTarget.b = 0.0f;
	mTarget.a = 0.0f;
	mSpeed.r  = 0.0f;
	mSpeed.g  = 0.0f;
	mSpeed.b  = 0.0f;
	mSpeed.a  = 0.0f;
}
