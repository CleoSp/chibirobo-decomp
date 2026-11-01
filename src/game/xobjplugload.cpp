#include "game/xobjattachment.h"
#include "game/stagedata.h"
#include "game/cgame.h"

// XObjPlug::vf9C, the plug, then its wire effect from spawn 16
/**
 * @note Address: 0x800D9414
 * @note Size: 0x104
 */
extern "C" bool fn_800D9414(XObjPlug* object, CoordInfo* info)
{
	switch (object->mLoadState) {
	case 0:
		if (!fn_80091C80(object, info))
			break;
		++object->mLoadState;
	case 1:
		if (!object->mWire.vf9C(CoordInfo(GetSpawnData(16))))
			break;
		Vec position = ((CXObj*)lbl_8025C400.mPlayer)->mMoveTranslate;
		fn_800DB810(&object->mCord, position.x, position.y, position.z);
		++object->mLoadState;
		break;
	}
	return object->mLoadState > 1;
}
