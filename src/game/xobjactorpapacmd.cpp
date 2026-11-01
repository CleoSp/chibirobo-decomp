#include "game/cobj.h"
#include "game/cgame.h"
#include "game/gmath.h"
#include "game/evt.h"
#include "game/xobjactor.h"

static inline CXObj* CommandTarget(s16 id)
{
	return lbl_8025C400._5D080 != NULL ? (CXObj*)fn_8007F570(&lbl_8025C400._5D080, id) : NULL;
}

/**
 * @note Address: 0x8012955C
 * @note Size: 0x210
 */
extern "C" s32 vf124__13XObjActorPapaFlPl(XObjActorPapa* object, s32 count, s32* command)
{
	fn_801070A4(object, count, command);
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			fn_8010771C(object, 0.08726646f);
			break;
		case 1:
			fn_801076B0(object, (s32) & ((CXObj*)lbl_8025C400.mPlayer)->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (CommandTarget(command[2]) != NULL) {
				fn_801076FC(object, (s32)&CommandTarget(command[2])->mMoveTranslate, DEG2RAD * (f32)command[3]);
			}
			break;
		case 3:
			fn_801076DC(object, DEG2RAD * (f32)command[2], DEG2RAD * (f32)command[3]);
			break;
		}
		break;
	default:
		if (count < 1)
			return 0;
		object->mCmd[0] = command[0];
		if (count < 2)
			object->mCmd[1] = 0;
		else
			object->mCmd[1] = command[1];
		object->mCmd[2] = command[2];
		object->mCmd[3] = command[3];
		object->mCmd[4] = command[4];
		object->mCmd[5] = command[5];
		object->mCmd[6] = command[6];
		object->mCmd[7] = command[7];
		object->mCmd[8] = command[8];
		object->mCmd[9] = command[9];
		break;
	}
	return 0;
}
