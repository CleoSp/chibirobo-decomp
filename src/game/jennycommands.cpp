#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

/**
 * @note Address: 0x8012305C
 * @note Size: 0x24C
 */
extern "C" s32 HandleEventCommand__14XObjActorJennyFlPl(XObjActorJenny* object, s32 count, s32* command)
{
	HandleEventCommand__9XObjActorFlPl(object, count, command);
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			ResetActorLook(object, 0.08726646f);
			break;
		case 1:
			SetActorLookAtPosition(object, &gGame.GetPlayer()->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(command[2]) != NULL) {
				SetActorLookTarget(object, &FindObj(command[2])->mMoveTranslate, DEG2RAD * command[3]);
			}
			break;
		case 3:
			SetActorLookAngle(object, DEG2RAD * command[2], DEG2RAD * command[3]);
			break;
		}
		break;
	default:
		if (count < 1) {
			return 0;
		}
		object->mCmd[0] = command[0];
		if (count < 2) {
			object->mCmd[1] = 0;
		} else {
			object->mCmd[1] = command[1];
		}
		for (int i = 0; i < 8; i++) {
			object->mCmd[i + 2] = command[i + 2];
		}
		if (object->mCmd[1] == 0xD0) {
			object->mCmd[19] = (s32)FindObj(object->mCmd[2]);
		}
		break;
	}
	return 0;
}

/**
 * @note Address: 0x801232A8
 * @note Size: 0x4
 */
void XObjActorJenny::StateNone(u32 frames)
{
}
