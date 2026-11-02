#include "game/xchr.h"
#include "game/texturecopy.h"
#include "game/stagedata.h"

// XObjPlug, the plug chibi robo pulls around: its load and init

extern "C" {

// XObjPlug::vf9C, the plug, then its wire effect from spawn 16
/**
 * @note Address: 0x800D9414
 * @note Size: 0x104
 */
BOOL fn_800D9414(XObjPlug* object, const CoordInfo& info)
{
	switch (object->mLoadState) {
	case 0:
		if (!fn_80091C80(object, &info)) {
			break;
		}
		object->mLoadState++;
	case 1:
		if (object->mWire.vf9C(CoordInfo(GetSpawnData(16)))) {
			Vec pos = lbl_8025C400.GetPlayer()->mMoveTranslate;
			fn_800DB810(&object->mCord, pos.x, pos.y, pos.z);
			object->mLoadState++;
		}
		break;
	}
	return object->mLoadState > 1;
}

// XObjPlug::vf94
/**
 * @note Address: 0x800D9A54
 * @note Size: 0x28
 */
void fn_800D9A54(XObjPlug* object)
{
	object->mStatus.mFlags &= ~0x0400000000000000ULL;
}

/**
 * @note Address: 0x800D9A7C
 * @note Size: 0x8
 */
void fn_800D9A7C(XObjPlug* object, f32 speed)
{
	object->mCord.mSpeed = speed;
}

// XObjPlug::vf90
/**
 * @note Address: 0x800D9A84
 * @note Size: 0x8C
 */
void fn_800D9A84(XObjPlug* object, u32 id)
{
	fn_800929C4(object, id);
	fn_800DBA7C(&object->mCord, 20, 0.0f, 0.0f, 0.0f, 0.5f);
	object->mCord.mColor = 0x141414FF;
	object->mCord._60    = 0x141414FF;
	object->mCord.mWidth = 0.2f;
	object->mLoadState   = 0;
	object->mWire.vf90(id);
}

} // extern "C"
