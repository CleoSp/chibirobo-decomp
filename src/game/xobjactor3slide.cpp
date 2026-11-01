#include "game/xobjactor.h"
#include "game/cgame.h"
#include <math.h>

extern "C" {
extern Vec lbl_8037FE40;
}

// todo: cleanup
static inline Vec PlacedPos(XObjActor3* actor)
{
	UnkStageObj* obj = actor->mCoord._00;
	Vec pos          = lbl_8037FE40;
	pos.x            = obj->mPos[0] / 100.0f;
	pos.y            = obj->mPos[1] / 100.0f;
	pos.z            = obj->mPos[2] / 100.0f;
	return pos;
}

/**
 * @note Address: 0x8010BC0C
 * @note Size: 0x370
 */
// slides 10 to the left from 0.42 of the day and back at half the day
extern "C" void fn_8010BC0C(XObjActor3* actor, u32 frames)
{
	f32 time = lbl_8025C400.mTimeState.mTime;
	switch (actor->_B80) {
	case 0:
		if (time >= 0.42f && time < 0.5f) {
			++actor->_B80;
		}
		break;
	case 1:
		actor->mMoveTranslate.x = actor->mMoveTranslate.x - 0.2f * (f32)frames;
		if (actor->mMoveTranslate.x <= PlacedPos(actor).x - 10.0f) {
			actor->mMoveTranslate.x = PlacedPos(actor).x - 10.0f;
			++actor->_B80;
		}
		break;
	case 2:
		if (time >= 0.5f) {
			++actor->_B80;
		}
		break;
	case 3:
		actor->mMoveTranslate.x = actor->mMoveTranslate.x + 0.2f * (f32)frames;
		if (actor->mMoveTranslate.x >= PlacedPos(actor).x) {
			actor->mMoveTranslate.x = PlacedPos(actor).x;
			actor->_B80             = 0;
		}
		break;
	}
}
