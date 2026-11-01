#include "game/cobj.h"
#include "game/cgame.h"
#include "game/cjobjinlineattach.h"
extern "C" {
#include <sysdolphin/mtx.h>
}

// the plug code's anchor, a little off the joint at 0, 1, 0, 1, 1
/**
 * @note Address: 0x800D909C
 * @note Size: 0x32C
 */
extern "C" void fn_800D909C(void*, Vec* position, Vec* rotation, Vec* scale)
{
	CXObj* player = (CXObj*)lbl_8025C400.mPlayer;
	s32 path[5]   = { 0, 1, 0, 1, 1 };
	Mtx matrix;
	CJObj joint(FindJointByPath(player->mJObj, path, 5));
	if (joint.mJObj)
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), matrix);
	static CVec offset(0.0f, 0.0f, -0.4f);
	if (position)
		PSMTXMultVec(matrix, &offset, position);
	if (rotation)
		HSD_MtxGetRotation(matrix, rotation);
	if (scale)
		HSD_MtxGetScale(matrix, scale);
}
