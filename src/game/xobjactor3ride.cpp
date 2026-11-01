#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cgame.h"
extern "C" {
#include <sysdolphin/mtx.h>
}

/**
 * @note Address: 0x8010AF74
 * @note Size: 0x420
 */
// puts chibi robo on the actor or lets him off, from an event command
extern "C" void fn_8010AF74(XObjActor3* actor)
{
	XObjChr* player = lbl_8025C400.mPlayer;
	CJObj jobj;
	switch (actor->_B94) {
	case 1:
		switch (actor->_B98) {
		case 1:
			actor->_B8C.mFlags |= 0x40;
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x40000000;
			player->mPadReset = 0;
			break;
		default: {
			actor->_B8C.mFlags &= ~0x40;
			s32 path[2] = { 0, 0 };
			jobj.Attach(FindJointByPath(actor->mJObj, path, 2));
			CVec pos(GetJointPosition(jobj.mJObj));
			player->SetPos(pos, 0);
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x20000000;
			player->_980.mFlags |= 7;
			player->mPadReset = 1;
			HSD_JObjClearMtxFlags(player->mJObj);
			break;
		}
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	}
}
